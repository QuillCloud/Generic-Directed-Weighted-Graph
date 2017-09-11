// test for printNodes()

#include "Graph.h"

int main() {

    // make a graph
    gdwg::Graph<unsigned int, int> g;
    gdwg::Graph<unsigned int, int> b(g);
    if (b.addNode(1))
        std::cout << "add 1 to b" << std::endl;
    if (g.addNode(1))
        std::cout << "add 1 to g" << std::endl;

}