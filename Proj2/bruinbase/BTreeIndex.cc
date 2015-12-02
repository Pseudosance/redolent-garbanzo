/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <iostream>
#include <stdio.h>

#include <queue>

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = 1;
    treeHeight = 0;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
    RC     rc;
  // open the index file 
  if ((rc = pf.open(indexname, mode)) < 0) { 
    fprintf(stderr, "Error: Failed to open index file %s \n", indexname.c_str());
    return rc;
  }
    
  // if page file opened is empty (empty tree)
  if(!pf.endPid()){
      // Init the tree vars (necessary because could use same BTreeIndex instance to close and open a new page)
      treeHeight = 0;
      rootPid = 1;
      //return 0;
      // Need to preserve height and root pid so it still exists between BruinBase uses
        // Really, only on close....
  }
  else{
      // Buffer to read page file into in order to determine root pid and tree height
      int treeData[(PageFile::PAGE_SIZE/sizeof(int))]; // TODO: Despite having a buffer size of 256, I really only need the first 2 slots. However, have to read in entire page file?
      
      // Read into treeData 
      if ((rc = pf.read(0, treeData)) < 0) { 
          fprintf(stderr, "Error: Failed to read the pagefile into buffer\n");
          return rc;
      }
      rootPid = treeData[0];
      treeHeight = treeData[1];
  }
  
    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    // Before the file closes, need to save the rootPid and treeHeight variables onto disk
    int treeData[(PageFile::PAGE_SIZE/sizeof(int))]; // TODO: Despite having a buffer size of 256, I really only need the first 2 slots. However, have to write to entire page file?
    treeData[0] = rootPid;
    treeData[1] = treeHeight;
    pf.write(0, treeData);
    
    return pf.close();
}

// Recommended to use recursion 
//////////////////////////////////////// INSERT HELPERS ////////////////////////////////////////////////////////////

RC BTreeIndex::recursiveInsert(int key, const RecordId& rid, PageId currentNode, int height, PageId& newNode, int& newKey){

    RC rc; // Return value
    
    // Base case when height = 0 (e.g. at leaf node)
    if(height == 0){
        BTLeafNode leafNode;
        leafNode.read(currentNode, pf);
        
        // Check if room for node, if so, insert
        if(leafNode.insert(key, rid) != 0){
           // cout << endl << endl << "Need to split leafnode" << endl;
            
            // If fails, (e.g. in this if statement it failed), need to split
            // If split that means need to insert new key and page ID into a nonleaf
            // The node this needs to be inserted into was the currentNode one level up of the recursive function
            // Meaning when we return we will be on that node
            // Pass the newSiblingKey and pageId of the newSiblingLeaf up a level with parameters (newNode, newKey)

           BTLeafNode newSiblingLeaf;
           newSiblingLeaf.initBuffer();
           if((rc = leafNode.insertAndSplit(key, rid, newSiblingLeaf, newKey)) != 0)
           {
               fprintf(stderr, "Error: Insert and Split on leaf failed\n");
               return rc;
           }
           newNode = pf.endPid();
           newSiblingLeaf.write(newNode, pf);
           leafNode.setNextNodePtr(newNode);
           
           // Special case, never had a real "root" we never recursed so won't hit code to increment the level and add new parent node
           if(currentNode == rootPid){
               // If the currentNode is the rootPid and we are a leaf node and splitting
              
                BTNonLeafNode rootNode; 
                rootNode.initBuffer();
                rc = rootNode.initializeRoot(rootPid, newKey, newNode);
                rootPid = pf.endPid();
                rc = rootNode.write(rootPid, pf);
                newKey = -1;
                newNode = -1;
                treeHeight +=1;
                
                //rootNode.printContents();
           }

        }
        // Write to node
        return leafNode.write(currentNode,pf);      // **** CHECK: changed pid to "currentNode"

    }
    
    
    // If height is > 0, then recursively go down correctly.
    PageId nextNode; 
    BTNonLeafNode node;
    node.read(currentNode, pf);
    if((rc = node.locateChildPtr(key, nextNode)) != 0)
    {
        fprintf(stderr, "Error: locate child ptr failed\n");
        return rc;
    }

    rc = recursiveInsert(key, rid, nextNode, height-1, newNode, newKey);
    
    // We are on the way back up now
    // Check if newNode and newKey are set (meaning we returned from a split action)
    if(newNode != -1){
        // Since not null, we needa insert newNode(pageId) and newKey into the currentNode
        if(node.insert(newKey, newNode) != 0){
            // if we needa split the nonLeafNode
            BTNonLeafNode newSiblingNode;
            newSiblingNode.initBuffer();
            if((rc = node.insertAndSplit(newKey, newNode, newSiblingNode, newKey)) != 0)
            {
                fprintf(stderr, "Error: Insert and Split on nonLeaf failed\n");
                return rc;
            }
            newNode = pf.endPid();
            rc = newSiblingNode.write(newNode, pf);
            
            // Note Special Case: If height == treeHeight and we need to split, then need a new root node (special because a return from here will exit recursiveInsert and thus loose the newNode and newKey values)
            if(treeHeight == height && currentNode == rootPid){
                BTNonLeafNode rootNode; 
                rootNode.initBuffer();
                rc = rootNode.initializeRoot(rootPid, newKey, newNode);
                rootPid = pf.endPid();
                rc = rootNode.write(rootPid, pf);
                treeHeight+=1;
            }
            
            
            return rc;                
        }
        else{ // else If node doesn't split, we should reset these parameters to null
            newNode = -1;
        }
        
        rc = node.write(currentNode, pf);
    }
    
    return rc;   

}
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    //cout << "in Insert with key= " << key << endl;
    RC rc = 0;
    

    //if(treeHeight > 0){ // Tree is not empty, need to recursively work our way to appropriate leaf node
        PageId newNode = -1;
        int newKey = -1;
        return recursiveInsert(key, rid, rootPid,treeHeight, newNode, newKey);
   // }
 /*   else if (treeHeight == 0) { // Tree is empty, so insert root....
        cout << "Tree Height was empty, so inserting at root " << endl;
        return insertOnEmptyTree(key, rid);
    }    
    */
    return 0;
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
    RC rc = 0;
    PageId pid = rootPid;
    int eid = -1;
    BTNonLeafNode nonLeafNode;
    BTLeafNode leafNode;
    
    //cout << searchKey << " Is the searchKey Parameter" << endl;
    //cout << treeHeight << " is treeHeight" << endl;
    
    int height;
    
    // first, traverse all the non-leaf nodes
    if(treeHeight > 0){
        for (height = 0; height < treeHeight; height++) {
            rc = nonLeafNode.read(pid, pf);
    
            if (rc < 0) {
                cout << "ZOINKS ! :(" << endl;
                return rc;
            }
            rc = nonLeafNode.locateChildPtr(searchKey, pid);
            if (rc < 0) {
                cout << "entered the bad zone :(" << endl;
                return rc;
            }
        }
    }
    
    // if we've reached this point, we are at the leaf nodes
    
    // repeat same code but with leaf node!
    
    rc = leafNode.read(pid, pf);
    if (rc < 0) {
        cout << "oh no :(" << endl;
        return rc;
    }

    rc = leafNode.locate(searchKey, eid);
    if (rc < 0) {
        cursor.pid = pid;
        cursor.eid = eid;
        return RC_NO_SUCH_RECORD;
    }
    
    cursor.pid = pid;
    cursor.eid = eid;
    
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */ 
                            //  (pid, eid)         key   (pageid, slotid)
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    //cout << "Inside readForward" << endl;
    //cout << "(cursor.pid, cursor.eid) = (" << cursor.pid << ", " << cursor.eid << " )" << endl; 
    if(cursor.pid == -1){
        return RC_END_OF_TREE;
    }
    RC rc;
    BTLeafNode node;
    // Read pair
    if ((rc = node.read(cursor.pid, pf)) < 0) { 
          fprintf(stderr, "Error: Failed to read get node data\n");
          return rc;
    }
    if ((rc = node.readEntry(cursor.eid, key, rid)) < 0) { 
          fprintf(stderr, "Error: Failed to read tuple data\n");
          return rc;
    }
    
    // Move forward cursor (the way I set up eid use in Node was that each eid was a byte of the buffer, so 12 bytes = 3 ints  which is 1 entry)
        // Check if pid, eid was last entry in node, if so get next node and change the pid and eid of cursor
        int curKey = (cursor.eid+12)/12;
        int keyCount = node.getKeyCount();
        if(curKey >= keyCount)
        {
            cursor.pid = node.getNextNodePtr();
            cursor.eid = 0;
        }
        // else increment eid by 12 
        else{
            cursor.eid += 12;
        }
        
    return 0;
}

void BTreeIndex::printTree(){
    queue<PageId> toVisit;
    toVisit.push(rootPid);
    cout << "Root Pid: " << rootPid << endl;
    bool leaf = false;
    int buffer[256];
    while(toVisit.size() > 0){
        PageId visiting = toVisit.front();
        toVisit.pop();
        
        pf.read(visiting, buffer);
        
        if(visiting == rootPid){
            cout << " PRINTING ROOT NODE " << endl;
            
            if(treeHeight == 0)
                leaf = true;
            else
                leaf = false;
        }
        else if(buffer[255] == -2){
            cout << " PRINTING NON LEAF NODE " << endl;
            leaf = false;
        }
        else{
            cout << " PRINTING LEAF NODE " << endl;
            leaf = true;
        }
        
        cout << "-----------------------------" << endl;
        
        if(leaf){
            for(int i = 0; i < 253; i+=3){
                cout << "(" << buffer[i] << ", " << buffer[i+1] << ") " << " key = " << buffer[i+2] << endl;
                
            }
        }
        else{
            cout << "Pid: " << buffer[0] << endl;
            toVisit.push(buffer[0]);
            for(int i = 1; i < 254; i+=2){
                cout << "key = " << buffer[i] << " and pid = " << buffer[i+1] << endl;
                if(buffer[i+1] != -2)
                    toVisit.push(buffer[i+1]);
            }
        }

    }
}