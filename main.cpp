// test for printNodes()

#include "Graph.h"

int main() {

    // make a graph
    try {
        gdwg::Graph<unsigned int, int> a;
        gdwg::Graph<unsigned int, int> b(a);
        a.addNode(1);
        a.addNode(2);
        a.addNode(3);
        a.addEdge(1, 2, 12);
        a.addEdge(2, 3, 23);
        a.addEdge(1, 3, 23);
        a.addEdge(1, 2, 22);
        a.deleteNode(2);
        a.addNode(2);
        a.addEdge(1, 2, 12);
    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }

}