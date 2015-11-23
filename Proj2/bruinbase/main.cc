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
cout << "Test 2" << endl;

	BTLeafNode test;
	BTLeafNode test2;
  
  RecordId r;
	r.sid = 1;
	r.pid = 1;

	int testkey = -1;

  for(int i = 0; i < 85; i++){
    if(i >= 43){
      test.insert(i+1, r);
    }
    else{
    test.insert(i, r);
    }
  }


	test.insertAndSplit(43, r, test2, testkey);

	cout << "First Node:" << endl;
	printLeafContents(test);
	cout << "Second Node:" << endl;
	printLeafContents(test2);
	cout << "Test Key: " << testkey << endl;

	cout << endl;
  return 0;
}
