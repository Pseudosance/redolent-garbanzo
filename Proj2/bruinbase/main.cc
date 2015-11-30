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

     
     return 0;

}
