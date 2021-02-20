# DS ASSIGNMENT 3  

## Execution  
1) Compile: `g++ -o main main.cpp`  
2) Run (to print output on terminal): `./main <input_file>`  
3) Run (to save output to a file): `./main <input_file> > <output_file>`  

## Implementation
Queries implemented: INSERT, FIND, COUNT, RANGE.  
Call print_tree() function to see the structure of the tree.  
Call print_allkeys() function to see all values inserted in the tree.  
To change the order (max no. of keys in a node) of the tree (order >= 2), change the MAX_KEYS definition (though not well tested for order!=2).  
Each tree node stores: MAX_KEYS no. of keys, MAX_KEYS+1 no. of child-node pointers, pointer to the parent-node, pointer to the next (right) sibiling-node (only for leaf nodes) and an is_leaf flag.  

## Assumptions
Input files should be in the same folder.  
Output file would be created in the same folder.  
Input file should be in the same format as given in the doc: keywords in capital letters, each query on a new line, each element in a query has 1 space in between.  
Here order means max np. of keys in any node.  
Code tested for only order=2 (as given in doc) though implemented for other orders too.  
