/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include "BTreeIndex.h"
#include <assert.h>
#include <cstdio>
#include <iostream>
using namespace std;

void printLeafContents(BTLeafNode n)
{
    int keyCount = n.getKeyCount();
    
    if (keyCount == 0)
    {
        cout << "This node is empty" << endl;
        return;
    }
    
    RecordId rid;
    int key;
    
    cout << "Key Count: " << keyCount << endl;
    
    for (int i = 0; i < keyCount * 12; i+=12 )
    {
        n.readEntry(i, key, rid);
        cout << "Entry: " << i << " "
        << "Key: " << key << " "
        << "RecordId (pid,sid): " << "(" << rid.pid << "," << rid.sid << ")" << endl;
    }
}


int main()
{
    // run the SQL engine taking user commands from standard input (console).
    SqlEngine::run(stdin);

    // BTNonLeafNode t1;
    // PageId p = 1;
    // t1.insert(3, 1);
    // t1.insert(1, 2);
    // t1.insert(5, 3);
    // t1.insert(4, 5);
    // t1.insert(2000,69); 
    // t1.printContents();
    // cout << endl;



    // Test 4 splitting non leaf
     // cout << "Test 4 " << endl << "-----------------------------------" << endl;
    // t1.insert(8, 2);
    // t1.insert(18, 4);
    // t1.insert(22, 6);
    // t1.insert(102, 8);
    // t1.insert(2, 12);

    // PageId r;
    // r = 1;
    // int testkey = -1;

    // BTNonLeafNode test;
    // BTNonLeafNode test2;

    // for (int i = 0; i <127; i++) {
    //     if (i >= 64){
    //         test.insert(i+1, r);
    //     }
    //     else{
    //         test.insert(i, r);
    //     }
    // }
    
    // test.insertAndSplit(64, r, test2, testkey);
    
    // cout << "First Node:" << endl;
    // test.printContents();
    // cout << "Second Node:" << endl;
    // test2.printContents();
    // cout << "Test Key: " << testkey << endl;

    // cout << "TESTING INITIALIZE ROOT" << endl;
    // BTNonLeafNode nonLeaf2;
    // PageId initPid = 100;
    // PageId initPid2 = 200;
    // int initKey = 1;

    // assert(nonLeaf2.initializeRoot(initPid, initKey, initPid2) == 0);

    // RecordId rid;
    // rid.pid = 10;
    // rid.sid = 12;
    //IndexCursor cursor;
    
     BTreeIndex index = BTreeIndex();
     assert(index.open("testingindex",'w') == 0);

     RecordId rid;
     rid.pid = 1;
     rid.sid = 2;
     int temp;
     temp = index.insert(1, rid);
    // // * Insert (key, RecordId) pair to the index.
    // // * @param key[IN] the key for the value inserted into the index
    // // * @param rid[IN] the RecordId for the record being inserted into the index
    // // * @return error code. 0 if no error
    //  cout << temp << endl;

     IndexCursor cursor;

     temp = index.locate(1, cursor);
     cout << temp << "temp1" << endl;
     cout << cursor.pid << endl;
     cout << cursor.eid << endl;

     IndexCursor cursor2;

     index.insert(2, rid);
     temp = index.locate(2, cursor2);
     cout << temp << "temp2" << endl;
     cout << cursor2.pid << endl;
     cout << cursor2.eid << endl;

     // IndexCursor cursor3;

     // index.insert(3, rid);
     // temp = index.locate(3, cursor3);
     // cout << temp << "temp3" << endl;
     // cout << cursor3.pid << endl;
     // cout << cursor3.eid << endl;

    // int temp;
    // for(int i = 1; i <= 85; i++){
    //     assert(index.insert(i,rid) != -1);
    //     index.locate(i,cursor);
    //     cout << i;
    //     //temp = index.locate(i, cursor);
    //     //cout << "***" << temp << endl;
    //     cout << "cursor pid: " << cursor.pid << " | cursor eid: " << cursor.eid << endl << "------" << endl;
    // }
    // assert(index.close() == 0);

    // BTNonLeafNode t2;
    // int midkey = -1;
    // t1.insertAndSplit(2, 12, t2, midkey);
    // cout << "t1 has" << endl;
    // t1.printContents();
    // cout << "t2 has" << endl;
    // t2.printContents();
    // cout << endl << "midkey is " << midkey << endl;
    // // Test 5 locateChildPtr
    // cout << "Test 5 " << endl << "-----------------------------------" << endl;;
    // cout << "Looking through node t2, enter a key to search for" << endl;
    // while(1)
    // {
    //     PageId locate;
    //     int search;
    //     cin >> search ;
    //     t2.locateChildPtr(search, locate);
    //     cout << "searching for key: " << search << endl <<"page id to follow is " << locate << endl;
    // }

    // PageId pid;

    // cout << "TESTING NONLEAFNODES" << endl;
    // BTNonLeafNode nonLeaf;
    // cout << "#Keys in Non-leaf node: " << nonLeaf.getKeyCount() << endl;
    // pid = 0;
    // assert(nonLeaf.insert(99, pid) == 0);
    // cout << "Inserted key = 99, into pid = 0" << endl;
    // cout << "#Keys in leaf node: " << nonLeaf.getKeyCount() << endl;
    // assert(nonLeaf.getKeyCount() == 1);

    // PageId pid2;
    // pid2 = 0;
    // int key2 = 85;
    // assert(nonLeaf.insert(key2, pid2) == 0);
    // assert(nonLeaf.getKeyCount() == 2);

    // PageId pid3;
    // pid3 = 0;
    // int key3 = 100;
    // assert(nonLeaf.insert(key3, pid3) == 0);
    // assert(nonLeaf.getKeyCount() == 3);

    // PageId pid4;
    // pid4 = 0;
    // int key4 = 101;
    // assert(nonLeaf.insert(key4, pid4) == 0);
    // assert(nonLeaf.getKeyCount() == 4);

    // PageId pid5;
    // pid4 = 1;
    // int key5 = 102;
    // assert(nonLeaf.insert(key5, pid5) == 0);
    // assert(nonLeaf.getKeyCount() == 5);

    // int pidTest;
    // nonLeaf.locateChildPtr(99, pidTest);
    // cout << "pidTest = " << pidTest << endl;
    // nonLeaf.locateChildPtr(85, pidTest);
    // cout << "pidTest = " << pidTest << endl;
    // nonLeaf.locateChildPtr(100, pidTest);
    // cout << "pidTest = " << pidTest << endl;
    // nonLeaf.locateChildPtr(101, pidTest);
    // cout << "pidTest = " << pidTest << endl;
    // nonLeaf.locateChildPtr(102, pidTest);
    // cout << "pidTest = " << pidTest << endl;

    // cout << "TESTING INITIALIZE ROOT" << endl;
    // BTNonLeafNode nonLeaf2;
    // PageId initPid = 100;
    // PageId initPid2 = 200;
    // int initKey = 1;

    // assert(nonLeaf2.initializeRoot(initPid, initKey, initPid2) == 0);

    // cout << "TEST 5 ------------------------------" << endl; 
    // BTNonLeafNode leaf5;  
    // cout << "count(0): " << leaf5.getKeyCount() << endl;    
    // PageId pid = 100;
    // PageId pid2 = 200;
    // int key = 1;
    // assert(leaf5.initializeRoot(pid,key,pid2) == 0);
    // cout << "count(1): " << leaf5.getKeyCount() << endl;  
    
    // pid = -1;
    // assert(leaf5.locateChildPtr(1,pid) == 0);
    // cout << "locateptr 0: " << pid << endl;
    // assert(pid == 100);
    
    // assert(leaf5.locateChildPtr(2,pid) == 0);
    // cout << "locateptr 2: " << pid << endl;   
    // assert(pid == 200);
        
    // assert(leaf5.insert(4,300) == 0);
    // assert(leaf5.locateChildPtr(3,pid) == 0);
    // //cout << "locateptr 3: " << pid << endl;   
    // assert(pid == 200);
    
    // cout << "key: " << leaf5.getKeyCount() << endl;
    
    // // assert(leaf5.locate(0,eid) == 0);
    // // assert(eid == 0);
    
    // // assert(leaf5.locate(10,eid) != 0);
    // // assert(eid == 2);
    // //100 0 200 4 300 10 400
    // assert(leaf5.insert(10,400) == 0);
    
    // BTNonLeafNode leaf5split; 
    // int midkey;
    // leaf5.insertAndSplit(100,1000,leaf5split,midkey);
    // cout << "mid key: " << midkey << endl;
    // cout << "newcount leaf5: " << leaf5.getKeyCount() << endl;
    // cout << "newcount leaf5split: " << leaf5split.getKeyCount() << endl;    
    
    // leaf5.locateChildPtr(-1,pid);
    // cout << "@pid -1: " << pid << endl;
    // assert(pid == 100);
    
    // leaf5.locateChildPtr(1,pid);
    // cout << "@pid 1: " << pid << endl;
    // assert(pid == 200);
    
    // leaf5split.locateChildPtr(1,pid);
    // cout << "@pidsplit 1: " << pid << endl;
    // assert(pid == 200);
    
    // leaf5split.locateChildPtr(7,pid);
    // cout << "@pidsplit 7: " << pid << endl;
    // assert(pid == 300); 
    // leaf5split.locateChildPtr(60,pid);
    // cout << "@pidsplit 60: " << pid << endl;
    // assert(pid == 400);



    /*cout << "Testing Nodes" << endl;
     BTLeafNode leaf;
     //cout << "#Keys in leaf node: " << leaf.getKeyCount() << endl;
     RecordId rid;
     rid.pid = 0;
     rid.sid = 1;
     assert(leaf.insert(99,rid) == 0);
     //cout << "Inserted key = 99, into pid =0 sid=1" << endl;
     //cout << "#Keys in leaf node: " << leaf.getKeyCount() << endl;
     assert(leaf.getKeyCount() == 1);
     RecordId rid2;
     rid2.pid = 0;
     rid2.sid = 0;
     int key2 = 85;
     assert(leaf.insert(key2, rid2) == 0);
     //cout << "Inserted key = 85, into pid = 0, sid = 0" << endl;
     //cout << "#Keys in leaf node: " << leaf.getKeyCount() << endl;
     assert(leaf.getKeyCount() == 2);
     RecordId rid3;
     rid3.pid = 0;
     rid3.sid = 2;
     assert(leaf.insert(100, rid3) == 0);
     assert(leaf.getKeyCount() == 3);
     int eid;
     leaf.locate(99, eid);
     cout << "eid = " << eid << endl;
     leaf.locate(85, eid);
     cout << "eid = " << eid << endl;
     leaf.locate(100, eid);
     cout << "eid = " << eid << endl;
     /*
     cout << "checking the readEntry function..." << endl;
     int key = -1;
     RecordId ridRead;
     ridRead.pid = -1;
     ridRead.sid = -1;
     leaf.readEntry(0, key, ridRead);
     cout << "Results from readEntry (key = " << key << ") (rid.pid = " << ridRead.pid << ") rid.sid = " << ridRead.sid << endl;
     leaf.readEntry(12, key, ridRead);
     cout << "Results from readEntry (key = " << key << ") (rid.pid = " << ridRead.pid << ") rid.sid = " << ridRead.sid << endl;
     */
    // cout << "Test 2" << endl;
    
    // BTLeafNode test;
    // BTLeafNode test2;
    
    // RecordId r;
    // r.sid = 1;
    // r.pid = 1;
    
    // int testkey = -1;
    
    // for(int i = 0; i < 85; i++){
    //     if(i >= 43){
    //         test.insert(i+1, r);
    //     }
    //     else{
    //         test.insert(i, r);
    //     }
    // }
    
    // test.insertAndSplit(43, r, test2, testkey);
    
    // cout << "First Node:" << endl;
    // printLeafContents(test);
    // cout << "Second Node:" << endl;
    // printLeafContents(test2);
    // cout << "Test Key: " << testkey << endl;
    
    // cout << endl;
    return 0;
}
