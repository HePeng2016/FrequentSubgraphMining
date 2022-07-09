# Compile

      ./make

When 'make' command is fininshed,'GraphGen' file is generated.

# Comands

      ./GraphGen  GraphDataSet Frequency FrequentGraphDataSet [-d  FrequentGraphDataSetIndex]  [ -group SelectedIndex] [-T]

GraphDataSet is the input file with graph topology storage format,which is as follow:

      N  
      L1 
      L2
      ..
      M  
      I1 I2 EL1
      I2 I3 EL2
      ..
N is the number of vertexs. 
L1,L2 .. are the labels of each vertex. 
M is the number of edges.
I1, I2 indicates the index of the vertex that begins from zero. 
For example, if I1 equal 0, it's label corrosponds the first line L1. 
EL1,EL2 .. indicate the label of each edge. 

For example the simple two edges graph:

      3
      7
      5
      1
      2
      0 1 8
      1 2 9 
      
Indicated the two edges graph: 7-(8)-5-(9)-1
      

Frequency is the support percentage and its the value range from [0,1). This means that the frequent graph should appear at least the percentage of whole population. 

FrequentGraphDataSet : is the name of output file recorded frequent subgraphs using graph topology storage format. 

[-T] : In default, Only Case 1 prune method is used, if the '-T' is added, both Case 1 and Case 2 prune methods are used. 

[ -group SelectedIndex] : SelectedIndex is a file indicating the group information, the frequent graph mining is appied in each group seperatly. It is opital to be selected. 

The format of SelectedIndex file is as follow:

      41     125     0.975056
      42     124     0.954475
      42     125     0.971153
      42     126     0.914459
      42     127     0.982332
      42     128     0.994360
      42     130     0.940185

The first column indicates the identifier of the graph, the second column indicates the identifier of the group, the third column indicates how much this graph belongs to this group.


      ./GraphGen  MATCH GraphDataSet  MatchGraphFile output [-Verbose]
      

GraphDataSet is the name of the file for supergraphs with graph topology storage format.

MatchGraphFile is the name of the file for subgraphs to be matched with graph topology storage format.

output is the name of output file, the format of output file is as follow: 
     
      sub1 sup1  m1
      sub1 sup2  m2
      ..
      sub2 sup1  m33
      sub2 sup2  m34
      ..


The first column represents the ID of the subgraph, the second column represents the ID of the supergraph, the third column represents how many times this subgraph appears in this supergraph.
For example: 

      0   3   4
      0   4   6
      0   5   3
      0   6   3
     
The first line indicate the 0'th subgraph matched the 3'th supergraph, and the 0'th subgraph appears three times in the 3'th supergraph. 

If -Verbose is selected there are more detail information appears in the output file. The format of output file is as follow: 


      sub1 sup2  iso1   supvertex1 
      sub1 sup2  iso1   supvertex2 
      ..
      sub1 sup2  iso2   supvertex6
      sub1 sup2  iso2   supvertex8
      ..
      
The first column represents the ID of the subgraph, the second column represents the ID of the supergraph, the third column represents isomorphism ID indicating the mapping how this subgraph map to supergraph. The fourth column indicates the vertex ID of supergraph mapping to the vertex ID of subgraph,the order indicate the vertex ID of subgraph, the first line indicates the ID 0, so on. 
 For example: 
 
   
      0   3   1   16
      0   3   1   14
      0   3   2   17
      0   3   2   15
      .. .. 
      
To indicate that the 0'th subgraph matched 3'th supergraph,there are two isomorphisms,1'th and 2'th isomorphisms, the mappings of 1'th and 2'th isomorphisms are (0->16,1->14) (0->17,1->15).


      ./GraphGen  COUNT GraphDataSet
      
GraphDataSet is the input file, this command will return the number of graphs in the GraphDataSet file.  

      
     ./GraphGen  SUMMARY GraphDataSet
      

GraphDataSet is the input file, this command will return the array with two columns, one column shows the number of nodes for each graph and another column shows the number of edges for each graph.

      ./GraphGen  INTERSECT GraphDataSet1   GraphDataSet2  ..  -o output 
      
GraphDataSet1 GraphDataSet2 .. are the input file. The intersection graphs of GraphDataSet1   GraphDataSet2  .. with removing duplicates will be stored in the file output.  
 
# The demo commands. 
To obtain the count of graphs. 
     
      ./GraphGen  COUNT  C_340 
    
To obtain the node count and edge count for each graph in graphs.
     
      ./GraphGen  SUMMARY  C_340 > log 
     
     
To obtain the frequent graphs at the frequency threshold 0.5. The frequent graphs are stored in file FrequentGraphDataSet.
     
      ./GraphGen  C_340    0.5  FrequentGraphDataSet
     
To obtain the frequent graphs at the frequency threshold 0.5 with both Case 1 and Case 2 prune methods.  
 
      ./GraphGen  C_340    0.5  FrequentGraphDataSet   -T

To obtain graphs that match results. C_340 is the file for supergraphs, FrequentGraphDataSet is the subgraphs file, and the matched results are stored in file 'output'. 

    ./GraphGen  MATCH  C_340  FrequentGraphDataSet  output 
    
The matched results are stored as verbose format. 

    ./GraphGen  MATCH  C_340  FrequentGraphDataSet  output -Verbose




