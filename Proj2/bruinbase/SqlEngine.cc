/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"     // added for part d

using namespace std;

// external functions and variables for load file and sql command parsing
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
    fprintf(stdout, "Bruinbase> ");
    
    // set the command line input and start parsing user input
    sqlin = commandline;
    sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
    // SqlParser.y by bison (bison is GNU equivalent of yacc)
    
    return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
    RecordFile rf;   // RecordFile containing the table
    RecordId   rid;  // record cursor for table scanning
    
    RC     rc;
    int    key;
    string value;
    int    count;
    int    diff;
    
    // open the table file
    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
        fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
        return rc;
    }
    
    // scan the table file from the beginning
    rid.pid = rid.sid = 0;
    count = 0;
    while (rid < rf.endRid()) {
        // read the tuple
        if ((rc = rf.read(rid, key, value)) < 0) {
            fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
            goto exit_select;
        }
        
        // check the conditions on the tuple
        for (unsigned i = 0; i < cond.size(); i++) {
            // compute the difference between the tuple value and the condition value
            switch (cond[i].attr) {
                case 1:
                    diff = key - atoi(cond[i].value);
                    break;
                case 2:
                    diff = strcmp(value.c_str(), cond[i].value);
                    break;
            }
            
            // skip the tuple if any condition is not met
            switch (cond[i].comp) {
                case SelCond::EQ:
                    if (diff != 0) goto next_tuple;
                    break;
                case SelCond::NE:
                    if (diff == 0) goto next_tuple;
                    break;
                case SelCond::GT:
                    if (diff <= 0) goto next_tuple;
                    break;
                case SelCond::LT:
                    if (diff >= 0) goto next_tuple;
                    break;
                case SelCond::GE:
                    if (diff < 0) goto next_tuple;
                    break;
                case SelCond::LE:
                    if (diff > 0) goto next_tuple;
                    break;
            }
        }
        
        // the condition is met for the tuple.
        // increase matching tuple counter
        count++;
        
        // print the tuple
        switch (attr) {
            case 1:  // SELECT key
                fprintf(stdout, "%d\n", key);
                break;
            case 2:  // SELECT value
                fprintf(stdout, "%s\n", value.c_str());
                break;
            case 3:  // SELECT *
                fprintf(stdout, "%d '%s'\n", key, value.c_str());
                break;
        }
        
        // move to the next tuple
    next_tuple:
        ++rid;
    }
    
    // print matching tuple count if "select count(*)"
    if (attr == 4) {
        fprintf(stdout, "%d\n", count);
    }
    rc = 0;
    
    // close the table file and return
exit_select:
    rf.close();
    return rc;
}

/*
 when the user issues the command "LOAD movie FROM 'movieData.del'",
 you should create a RecordFile named movie.tbl (in the current working directory) and store all tuples in the file.
 If the file already exists, the LOAD command should append all records in the load file to the end of the table.
 Roughly, your implementation of the load function should open the input loadfile and the RecordFile,
 parse each line of the loadfile to read a tuple (possibly using SqlEngine::parseLoadLine()) and insert the tuple to the RecordFile.
 */
RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
     // Copied from the select function
    RecordFile rf;   // record for the table
    RecordId   rid;  // record cursor for table scanning
    BTreeIndex bTreeIndex;

    RC     rc;
    int    key;
    string value;

    // open the table file (Copied from select function, but with write attribute instead of read
    if ((rc = rf.open(table + ".tbl", 'w')) < 0) { // Create a record file named movie.tbl
        fprintf(stderr, "Error: Failed to create or open table %s \n", table.c_str());
        return rc;
    }

    if(index){
        if ((rc = bTreeIndex.open(table + ".idx", 'w') != 0)) {     // index failed to open
            fprintf(stderr, "Error: Failed to open index \n");
            return rc;
        }
    }

    // open file stream
    fstream loading (loadfile.c_str());
    if(loading.is_open()){
        string line;
        while(getline(loading, line))
        {
            // Append to table
            parseLoadLine(line, key, value);
            rc = rf.append(key, value, rid);
            if(index){
                if(rc < 0)
                    return rc;
                else
                    bTreeIndex.insert(key, rid);
            }
        }

        loading.close();
    }
    else{
        fprintf(stderr, "Error: Failed to open load file %s \n", loadfile.c_str());
        rc =  RC_FILE_OPEN_FAILED;
    }

    if(index)
        bTreeIndex.close();

    rf.close();

    return rc;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }
    
    // get the integer key value
    key = atoi(s);
    
    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }
    
    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }
    
    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }
    
    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }
    
    return 0;
}
