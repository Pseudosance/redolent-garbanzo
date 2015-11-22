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

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  SqlEngine::run(stdin);
    
    cout << "FIRST TEST (LEAF)" << endl;
    BTLeafNode leaf;
    cout << "count(0): " << leaf.getKeyCount() << endl;
    RecordId rid;
    rid.pid = 1;
    rid.sid = 10;
    assert(leaf.insert(1,rid) == 0);
    rid.pid = 2;
    rid.sid = 20;
    assert(leaf.insert(2,rid) == 0);
    
    int key;
    cout << "rid with a pid of 1: " << leaf.readEntry(0, key, rid) << endl;
    cout << "At eid 0: " << key << endl;

  return 0;
}
