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

void SqlEngine::printTuple(const int attr, const int key, int value)
{
    // fprintf the tuple
    switch (attr) {
        case 1:  // value
            fprintf(stdout, "%s\n", value.c_str());
            break;
        case 2:  // key
            fprintf(stdout, "%d\n", key);
            break;
        case 3:  // SELECT *
            fprintf(stdout, "%d '%s'\n", key, value.c_str());
            break;
    }
}

void SqlEngine::printCount(const int attr, const int count){
        if (attr == 4) {
            fprintf(stdout, "%d\n", count);
        }
}

// If 1 is returned we skip to next tuple, if 0, means we passed the check and print out tuple and increment count
int SqlEngine::conditionCheck(const SelCond::Comparator comparator, const int diff){
        switch (comparator) {
            case SelCond::GT:
                if (diff <= 0) 
                    return 1;
                break;
            case SelCond::EQ:
                if (diff != 0) 
                    return 1;
                break;
            case SelCond::LE:
                if (diff > 0) 
                    return 1;
                break;
            case SelCond::NE:
                if (diff == 0) 
                    return 1;
                break;
            case SelCond::GE:
                if (diff < 0) 
                    return 1;
                break;
            case SelCond::LT:
                if (diff >= 0) 
                    return 1;
                break;
        }
        
        return 0;
}

/*
The select() function is called when the user issues the SELECT command.
The attribute in the SELECT clause is passed as the first input parameter attr 
(attr=1 means "key" attribute, attr=2 means "value" attribute, attr=3 means "*", and attr=4 means "COUNT(*)"). 
The table name in the FROM clause is passed as the second input parameter table.
The conditions listed in the WHERE clause are passed as the input parameter conds
*/
RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
    RecordId   rid;  // cursor for the record; scans
    RecordFile rf;   // Record, File; has the table in it
    IndexCursor indexCursor;
    BTreeIndex bTreeIndex;
    
    RC     rc;
    int    diff;
    int    count;
    int    key;
    string value;
    bool has_Index = false;

    // look at the table file and open it
    if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
        fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
        return rc;
    }  

    // start at the beginning
    rid.pid = rid.sid = 0;
    count = 0;
    if (bTreeIndex.open(table+".idx", 'r')) // this indicates that there is NO INDEX
    {
        while (rid < rf.endRid()) {
           // we have the tuple at this point so read it in
            if ((rc = rf.read(rid, key, value)) < 0) {
                fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
                goto exit_select;
            }       

            // look at the tuple and check its conditions accordingly
            for (unsigned i = 0; i < cond.size(); i++) {
                // get diff between tuple val and condition val
                switch (cond[i].attr) {
                    case 1:
                        diff = strcmp(value.c_str(), cond[i].value);
                        break;
                    case 2:
                        diff = key - atoi(cond[i].value);
                        break;
                }
                
                // no condition met? SKIP THE TUPLE
                if(conditionCheck(cond[i].comp, diff))
                    goto next_tuple;
            }

            // the tuple matches a condition, so increment the tuple's counter!
            count++;            

            // fprintf the tuple
            printTuple(attr, key, value);
           
            // done with this tuple, so move it ahead
            next_tuple:
                ++rid;
        }       
        // if we have "select count(*)", then print that tuple
        printCount(attr, count);
        rc = 0;
    }
    else{ // We have an index
    /*
    If a SELECT query has one or more conditions on the key column and if the table has a B+tree, use the B+tree to help answer the query as follows:
       * If there exists an equality condition on key, you should always use the equality condition in looking up the index.
       * Queries which specify a range (like key >= 10 and key < 100) must use the index. 
         SqlEngine should try to avoid retrieving the tuples outside of this range from the underlying table by using the index.
       * You should not to use an inequality condition <> on key in looking up the B+tree.
       * You should avoid unnecessary page reads of getting information about the "value" column(s);
         for example, if ONLY "key" column(s) exist in a SELECT statement, or if travesing leaf nodes on B+tree returns the count(*).
    */
        has_Index = true;
        int skip_count = -1;
        int searchKey = -1;
        int cond_value = -1;
        int temp_attr = -1;
        // 0 is NOT_SET, 1 is EQ, 2 is NE, 3 is LT, 4 is GT, 5 is LE, 6 is GE
        SelCond::Comparator comparator = NOT_SET;

        // Find lowerbound key of our search
        for(unsigned i = 0; i < cond.size(); i++){
            cond_value = cond[i].value;
            temp_attr = cond[i].attr;
            comparator = cond[i].comp;
         
            // We only use the index on key conditions
            if(temp_attr != 1){
                continue;
            }
        
            //If there exists an equality condition on key, you should always use the equality condition in looking up the index.
            if(comparator == SelCond::EQ){
                searchKey = cond_value;
                break;
            }   

            //Queries which specify a range (like key >= 10 and key < 100) must use the index. 
            if(comparator == SelCond::GE || comparator == SelCond::GT){
                if(searchKey < cond_value)    // Don't just set searchKey = cond_value, because we may have other conditions we passed 
                    searchKey = cond_value;
            }
        }
       
        //Get an index cursor to point to the location of our lowerbound
        bTreeIndex.locate(searchKey, indexCursor);
        if(comparator == SelCond::NOT_SET){ // if comparator is NOT_SET, that means there were no conditions, so we would return all of that column
            indexCursor.eid = 0;
            indexCursor.pid = 1; // Furthest left leaf is pid = 1 due to our implementation (0 is tree data (height and rootpid), root is variable)
        }        

        // scan through the leaf nodes starting at our lowerbound and print out the tuple if we pass all conditions
        while(bTreeIndex.readForward(indexCursor, key, rid) == 0) // If readforawrd doesn't return 0, we reached the end
        {
            // Read in tuple
            if ((rc = rf.read(rid, key, value)) < 0) {
                fprintf(stderr, "Error: cannot read tuple from table \n", table.c_str());
                goto exit_select;
            }
            
            // See if tuple passes all our conditions
            for (unsigned i = 0; i < cond.size(); i++) 
            {
                temp_attr = cond[i].attr;
                cond_value = cond[i].value;
                comparator = cond[i].comp;
                
                // get diff between tuple val and condition val
                switch (temp_attr) 
                {
                    case 1:
                        diff = key - atoi(cond_value);
                        break;
                    case 2:
                        diff = strcmp(value.c_str(), cond[i].value);
                        break;
                }
                    
                // no condition met? SKIP THE TUPLE
                if(conditionCheck(comparator, diff))
                    goto index_next_tuple;
                
            }
            
            // If tuple passes all conditions, increment count and print right tuple values 
                // Note: Don't print if the attr = 4 (select count(*))            
            count++;
            // fprintf the tuple
            printTuple(attr, key, value);
            
            index_next_tuple:
                skip_count++; // For shits and giggles

        }     

        // If attr = 4 (select count(*))
        printCount(attr, count);
        rc = 0
        

    }

    // close the table file and return
    exit_select:
        // Close the BTreeIndex if opened
        if(has_Index)
            bTreeIndex.close();
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
