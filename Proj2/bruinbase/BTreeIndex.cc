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

using namespace std;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
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
    
    char buffer[PageFile::PAGE_SIZE]; //buffer to read index into
    
    RC rc = pf.open(indexname, mode);
    
    // as specified by spec: "error code. 0 if no error"...could not open
    if (rc != 0) {
        return RC_FILE_OPEN_FAILED;
    }
    
    // endPid() returns the id of the last page in the file (+ 1)
    
    // thus, if it's 0, then there are no pages in the file.
    
    if (pf.endPid() == 0) {
        treeHeight = 0;
        rootPid = -1;
        close();
        return open(indexname, mode);
    }
    
    else {
        if (pf.read(0, buffer) != 0) {      // failed to read
            return RC_FILE_READ_FAILED;
        }
        memcpy(&treeHeight, buffer, sizeof(int));
        memcpy(&rootPid, buffer + sizeof(int), sizeof(PageId));
    }
    
    return 0;   // no errors
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
    return 0;
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
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
    BTLeafNode leafNode;        // calls constructor
    BTNonLeafNode nonLeafNode;  // calls constructor
    
    if (rootPid == -1 || treeHeight == 0){
        return RC_FILE_SEEK_FAILED;     // tree is empty
    }
    
    PageId pid = rootPid;
    
    for (int i = 0; i < treeHeight - 1; i++) {      // will take you to leaf node
        nonLeafNode.read(pid, pf);                  // reads the pid into the nonLeafNode
        nonLeafNode.locateChildPtr(searchKey, pid);
    }
    
    // pid now has the pid of the leaf node we are trying to locate
    
    if (leafNode.read(pid, pf) != 0) {  // read pid into leaf node
        return RC_FILE_READ_FAILED;
    }
    
    int eid = -1;
    
    if (leafNode.locate(searchKey, eid) != 0) {
        return RC_FILE_SEEK_FAILED;
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
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
    return 0;
}
