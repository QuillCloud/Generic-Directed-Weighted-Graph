Requirement detail see "CS6771 Assignment Three 2017"

In this assignment, you will write a Generic Directed Weighted Graph (GDWG) with value-like semantics in C++. Both the data stored at a node and the weight stored at an edge will be of generic types. Both generic types may be different. For example, here is a graph with nodes storing std::string and edges weighted by int: 

gdwg::Graph<std::string,int> g;
Formally, this directed weighted graph G=(N,E) will consist of a set of nodes N and a set of weighted edges E. Give a node, an edge directed into it is called an incoming edge and an edge directed out of it is called an outgoing edge. The in-degree of a node is the number of its incoming edges. Similarly, the out-degree of a node is the number of its outgoing edges. Given a directed edge from src to dst, src → dst, src is the source node and dst is known as the destination node. G is a multi-edged graph, as there may be two edges from the same source node to the same destination node with two different weights. However, all nodes are distinct, as they contain different data values. 


