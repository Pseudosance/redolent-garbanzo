Tremaine Eto tremaineeto@ucla.edu
Mitchell Binning mitchellbinning@ucla.edu

 - Working under the assumption there are no duplicate key values.

 - Leaf nodes are implemented such that they use their entire buffer (holds 85 tuples, satisfying the 70 minimum). 

 - Non Leaf nodes are implemented such that they hold 127 keys and have 4 bytes of unused space each. (satisfying the 70 minimum)

 - The B+Tree stores its height and rootpid at pid=0. The tree sarts off as a leaf node (pid = 1). Upon filling the leaf, it splits into two leaves and spawns a non leaf. The non leaf is now the root with pid = 3. The two leaves are at pid = 1 and pid = 2. Etc...

 - The B+Tree's insert and split function was written recursively as recommended by the spec in order to keep track of the parent nodes.

 - It should be noted, should one play with the internals, that we used eid to be the index into the char* buffer. Meaning its a pain in the ass :). An eid of 12 for a leaf node is the second entry in the leaf node (The first is at eid =0 and is the sizeof(int)*3 = 12, thus the second entry begins at 12)

 - There may be some print statements left from debugging 0.o

 - The select with index criteria works in the following manner:
	- If equality
		- locate and print if found, exit....
		- (probably doesn't work on complex conditions with equals)
	- else, Determine the upper and lower bounds of the search
		- GT or GE sets a lowerbound
		- LT or LE sets an upperbound
	- if count(*), don't read tuples (probably means won't pass cases like: select count(*) from a_table where value = anything
	- find keys matching conditions the bounds
		- print tuples out and increment count as we go


