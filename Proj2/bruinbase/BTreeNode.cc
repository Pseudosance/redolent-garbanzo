#include "BTreeNode.h"
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <iostream>
using namespace std;

/*
 * Constructor to set buffer to a state we can check if unused
 */
BTLeafNode::BTLeafNode()
{
    memset(buffer, -1, PageFile::PAGE_SIZE);
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
    //cout << "leaf node Reading pid: " << pid  << endl;
    return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
    return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
    
    int count = 0;
    //cout << "enters getKeyCount()" << endl;
    // Need to index and iterate through buffer using int sizes
    int* bufferInts = (int *) buffer;
    // Calculate the increment in integer size to get to next pair
    int pairIncr = leafNode_pairSize/sizeof(int);
    for(int pairIndex = 0; pairIndex < leafNode_tupleLimit * pairIncr; pairIndex += pairIncr)
    {
        //cout << "for loop" << pairIndex << endl;
        /* Detect when buffer becomes unused */
        if(bufferInts[pairIndex] == -1)
            break;
        else
            count += 1;
    }
    
    return count;
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	// Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    //PageId* bufferPageIds = (PageId *) buffer;
	
	// Check if node is full
	if(bufferInts[leafNode_tupleLimit*3-1] != -1){ // 255 is last int, but thats sibling ptr
		return RC_NODE_FULL;
    }
    
	// Find first free space in Node to insert pair and find slot to insert new key
	int free_slot;
	int pageID = rid.pid;
	int slotID = rid.sid;
	int old_pageID = -1;
	int old_slotID = -1;
	int old_key = 0;    // should be 0?
	for(free_slot = 0; free_slot < (PageFile::PAGE_SIZE/sizeof(int))-1; free_slot+=3){
		if(bufferInts[free_slot] == -1){
            bufferInts[free_slot] = rid.pid;
            bufferInts[free_slot+1] = rid.sid;
            bufferInts[free_slot+2] = key;
			break;
        }
		// Key values, if greater than our key, then thats the slot for our new key, repeat process for old pair
		if(bufferInts[free_slot+2] > key)
		{
			old_pageID = bufferInts[free_slot];
			old_slotID = bufferInts[free_slot+1]; 
			old_key = bufferInts[free_slot + 2];
			bufferInts[free_slot] = pageID;
			bufferInts[free_slot+1] = slotID;
			bufferInts[free_slot+2] = key;
			key = old_key;
			pageID = old_pageID;
			slotID = old_slotID;
		}
	}
    
	// Have combed through all used slots
	if(old_pageID == -1 && old_slotID == -1){ // If old key is null, that means we didn't replace any old nodes and just inserted. 
        return 0;
    }
	// If old key is not null, we need to insert all the old shit into the free slot.
	bufferInts[free_slot] = old_pageID;
	bufferInts[free_slot+1] = old_slotID;
	bufferInts[free_slot+2] = old_key;
    
	return 0;
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	// Check if sibling provided is empty, if not, then error
	if(sibling.getKeyCount() != 0){
		return RC_NODE_FULL;
	}
	
	// Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    
    // Where the key should go
    int inOld_pos;
    locate(key, inOld_pos);
    inOld_pos = inOld_pos/12;
    // Find where to split at, leaf node holds 85 pairs, put 43 in left node and 42 in right (Value for bufferInts)
    int split_at = leafNode_tupleLimit/2 + 1; // 43
    bool inOld = false;
    if(inOld_pos < split_at){
        inOld = true;
        // If in old node, then 0-41 (42 tuples) go into old and the new node making 43 tuples
        split_at--;
    }
    
	// Middle byte pos of middle + 1 pair in buffer
	int bytePos_splitAt = split_at * leafNode_pairSize; // 129 * 4 = 516 (value for buffer)
	// size of buffer available to use after saving end for the sibling pointer
	int trueBufferSize = PageFile::PAGE_SIZE - sizeof(PageId); // 1024 - 4 = 1020
	// Remaining bytes in buffer (1024 - 4 - 516) = 504
	int remBytes = trueBufferSize - bytePos_splitAt;
	
	// Copy pairs 43 - 85 to the beginning of the new node's buffer
    memmove(sibling.buffer, buffer+bytePos_splitAt, remBytes);
	// Set the new nodes sibling pointer to old nodes sibling pointer 
    memmove(sibling.buffer+trueBufferSize, buffer+trueBufferSize, sizeof(PageId)); // last 4 bytes of the new nodes buffer is set to the last 4 bytes of the old nodes buffer
	// Set the old node's buffer from the 43rd node to the end as "empty" (e.g. -1)
    memset(buffer+bytePos_splitAt, -1, remBytes);
	// Set the old nodes sibling pointer to new node
    
    //setNextNodePtr(sibling);
	//bufferInts[255] = *(PageId*) &sibling;          // CHANGED FROM ORIGINAL!!!!!!
	
	// Insert new pair 
	if(inOld){
		insert(key, rid);
	}
	else{
		sibling.insert(key, rid);
	}
	
    int* siblingBufferInts = (int *) sibling.buffer;
	siblingKey = siblingBufferInts[2];
	return 0;
}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
    // Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    
    // iterate through the node looking for the searchKey.
        // TODO: Change to binary search for improved performance, it is hinted we should do so because the nodes are always sorted.
    int location;
    for (location = 0; location < 255 && bufferInts[location] != -1 && bufferInts[location+2] <= searchKey; location+=3){
        int key_pos = location+2;
       // cout << bufferInts[key_pos] << endl;
        if(bufferInts[key_pos] == searchKey)
        {
            // TODO: find out if should return location in char buffer or location in int buffer
            eid = location*4; // Multiply by 4 to have index entry in char buffer
            return 0;
        }
        if(bufferInts[key_pos] > searchKey)
           break;
    }
    
    
    
    eid = location*4; // Find out if should return location in char buffer or location in int buffer...
    return RC_NO_SUCH_RECORD;
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{ 
    if(eid < 0 || eid >= 1016)
        return RC_INVALID_CURSOR;
        
    // Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    // Adjust index entry to be for integer array
    int eid_intBuffer = eid/4;
    rid.pid = bufferInts[eid_intBuffer]; 
    rid.sid = bufferInts[eid_intBuffer+1];
    key = bufferInts[eid_intBuffer+2];
    
    return 0; 
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
    // Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    int siblingPtr = bufferInts[255];
    return siblingPtr;
    //return bufferInts[PageFile::PAGE_SIZE-1];
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
    // Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    bufferInts[255] = pid;
    return 0; 
}

void BTLeafNode::initBuffer(){
    memset(buffer, -1, PageFile::PAGE_SIZE);
}

// TODO: Redo insert and insert and split using locate, getNextNodePtr, and setNextNodePtr....
// TODO: Consider changing the buffer member variable to an int array instead of char arrary....

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/********************** NONLEAFNODE **********************/

/* 
 * Constructor
 */
BTNonLeafNode::BTNonLeafNode() {        // had to add in public for BTreeNode.h
    memset(buffer, -2, PageFile::PAGE_SIZE); //   from private: buffer[PageFile::PAGE_SIZE]
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf) {
    //return 0;
    //cout << "Nonleaf read pid = " << pid  << endl;
     return pf.read(pid, buffer);
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf) {
    //return 0;
    return pf.write(pid, buffer);
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount() {
        int count = 0;
        // Need to index and iterate through buffer using int sizes
        int* bufferInts = (int *) buffer;
        // Calculate the increment in integer size to get to next pair
        int pairIncr = nonLeafNode_pairSize/4;
        for (int pairIndex = 1; pairIndex < nonLeafNode_keyLimit * pairIncr; pairIndex += pairIncr)
        {
            // TODO: Remove cout
           // cout << "bufferInts[pairIndex] = " << bufferInts[pairIndex] << " with a pairIndex of " << pairIndex << endl;
            /* Detect when buffer becomes unused */
            if(bufferInts[pairIndex] == -2)
                break;
            else
                count += 1;
        }
        //cout << "Count = " << count << endl;
        return count;
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid) {
    
    // Get an int buffer because its easier to work with
    int* bufferInts = (int *) buffer;
    
    // Check if node is full
    if(bufferInts[nonLeafNode_keyLimit*2-1] != -2)      // is -2 correct?
        return RC_NODE_FULL;
    
    // Find first free space in Node to insert pair and find slot to insert new key
    int free_slot;
    int pageID = pid;
    int old_pageID = -1;
    int old_key;
    
    for (free_slot = 1; free_slot < (PageFile::PAGE_SIZE/sizeof(int))-1; free_slot+=2) {
        if (bufferInts[free_slot] == -2) {
            bufferInts[free_slot+1] = pid;
            bufferInts[free_slot] = key;
            break;
        }
        // Key values, if greater than our key, then that's the slot for our new key, repeat process for old pair
        if (bufferInts[free_slot] > key)      // this is 1, right?
        {
            old_pageID = bufferInts[free_slot+1];
            old_key = bufferInts[free_slot];        // this is 1, right?
            bufferInts[free_slot+1] = pageID;
            bufferInts[free_slot] = key;
            key = old_key;
            pageID = old_pageID;
        }
    }
    
    // Have combed through all used slots
    if (old_pageID == -1) // If old key is null, that means we didn't replace any old nodes and just inserted.
        return 0;
    
    // If old key is not null, we need to insert all the old shit into the free slot.
    bufferInts[free_slot+1] = old_pageID;
    bufferInts[free_slot] = old_key;
    
    return 0;
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey) {
    
    // Check if sibling provided is empty, if not, then error
    if(sibling.getKeyCount() != 0){
        return RC_NODE_FULL;
    }
    int keyInserting = key;
    // Get an int buffer because it's easier to work with
    int* bufferInts = (int *) buffer;
    // Find where to split at, non-leaf node holds 127 pairs, put 64 in left node and 63 in right (Value for bufferInts)
    int split_at = nonLeafNode_keyLimit/2 +1; // 64
    
    int inOld_pos;
    for(inOld_pos = 1; inOld_pos < nonLeafNode_keyLimit*2-1; inOld_pos+=2 ){
        //if(inOld_pos == 1)
            //cout << "inOld_pos inside loop with value 1" << endl;
        if(bufferInts[inOld_pos] >= key)
            break;
    }
    //cout << "inOld_pos = " << inOld_pos << endl;
    
    inOld_pos = inOld_pos/2 +1; //127
    bool inOld = false;
    if(inOld_pos < split_at){
        inOld = true;
        split_at--;     // split at = 63,    0-63 and new pair go into old (64 tuples), and 65 - 127  go into new
    }
    
    inOld_pos = inOld_pos*2 -1;
    split_at= split_at*2 -1; // 0-63 = 64 in left, position in bufferInts is 127 (start of 64th pair)
    
    if(inOld)
        midKey = bufferInts[split_at];
    else{
        if(inOld_pos == split_at)
            midKey = keyInserting;
        else
            midKey = bufferInts[split_at];
    }
    
    
    // Middle byte pos of middle + 1 pair in buffer
    int bytePos_splitAt = (split_at) * sizeof(int) +sizeof(int); // 508+4 = 512 (value for buffer)

    // Remaining bytes in buffer (1024 - 512) = 512
    int remBytes = PageFile::PAGE_SIZE - bytePos_splitAt;
    
    // Copy pairs 64 - 127 to the beginning of the new node's buffer
    memmove(sibling.buffer, buffer+bytePos_splitAt, remBytes);
    // Set the old node's buffer from the 64th node to the end as "empty" (e.g. -1)
    /*char *wipeFrom = buffer + bytePos_splitAt - sizeof(int);
    remBytes+=4;
    memset(wipeFrom, -2, remBytes);*/
    int i = bytePos_splitAt/4;
    i = i-1;
    //cout << i <<endl;
    for(i ; i < 256; i++){
        bufferInts[i] = -2;
    }
    
    
    // Insert new pair 
	if(inOld){
		insert(keyInserting, pid);
	}
	else{
        if(midKey != keyInserting)
		     sibling.insert(keyInserting, pid);
	}
    
    return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid){
    // page id | key | page id | key ... | pageid
    //cout << "In locateChildPtr" << endl;
    //cout << "SearchKey: " << searchKey << endl;
    char* memoryPointer = &(buffer[0]);
    
   // memoryPointer = memoryPointer + sizeof(int);     // skip first page id
    
    memcpy(&pid, memoryPointer, sizeof(PageId));
    //cout << "pid1: " << pid << endl;
    
    memoryPointer = memoryPointer + sizeof(PageId);     
    int keyPointedTo = 0;
    
    int* p = (int *) buffer;
    int keyCount = getKeyCount();
   // cout << "keycount is: " << keyCount << endl;
    while (keyCount > 0) {
        memcpy(&keyPointedTo, memoryPointer, sizeof(int));
    //    cout << "KeyPointedTo: " << keyPointedTo << endl;
        //cout << "test" << endl;
        
        if (searchKey < keyPointedTo) {
            return 0;      // pid's value is not reset here; success
        }
        
        if (keyPointedTo == 0) {
            return 0;      // pid's value is not reset here; success
        }
        
        memoryPointer = memoryPointer + sizeof(int);        // based on memcpy above, move 4 bytes
       
        memcpy(&pid, memoryPointer, sizeof(PageId));
     //   cout << "pid2: " << pid << endl;
        
        memoryPointer = memoryPointer + sizeof(PageId);
        
        keyCount -= 1;
    }
    
    return 0;       // success
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2){
    int* bufferInts = (int *) buffer;
    PageId* bufferPages = (PageId *) buffer;
    
    for(int i = 0; i < (PageFile::PAGE_SIZE/sizeof(int)); i++){
        bufferInts[i] = -2;
    }
    
    bufferPages[0] = pid1;
    bufferInts[1] = key;
    bufferPages[2] =  pid2;
    /*
    char* memoryPointer = &(buffer[0]);
    //memoryPointer = memoryPointer + sizeof(int);
    
    memcpy(memoryPointer, &pid1, sizeof(PageId));
    memoryPointer = memoryPointer + sizeof(PageId);
    
    memcpy(memoryPointer, &key, sizeof(int));
    memoryPointer = memoryPointer + sizeof(int);
    
    memcpy(memoryPointer, &pid2, sizeof(PageId));
    memoryPointer+= sizeof(PageId);
   // memset(memoryPointer, -2, (PageFile::PAGE_SIZE - sizeof(PageId) - nonLeafNode_pairSize));
    */
    //int initializedValue = 1;
    
   // memcpy(buffer, &initializedValue, sizeof(int));
    
    return 0;
}

void BTNonLeafNode::initBuffer(){
    int* bufferInts = (int *) buffer;
    PageId* bufferPages = (PageId *) buffer;
    
    for(int i = 0; i < (PageFile::PAGE_SIZE/sizeof(int)); i++){
        bufferInts[i] = -2;
    }

}

