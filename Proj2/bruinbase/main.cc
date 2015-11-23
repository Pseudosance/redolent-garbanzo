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
    cout << "Testing Nodes" << endl;
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
    return 0;
    
}
