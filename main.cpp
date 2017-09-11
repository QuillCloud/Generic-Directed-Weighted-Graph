// test for printNodes()

#include "Graph.h"

int main() {

    // make a graph
    try {
        gdwg::Graph<unsigned int, int> a;
        gdwg::Graph<unsigned int, int> b(a);
        if (a.addNode(1))
            std::cout << "add node success" << std::endl;
        else
            std::cout << "add node fail" << std::endl;
        if (b.addNode(1))
            std::cout << "add node success" << std::endl;
        else
            std::cout << "add node node fail" << std::endl;
        if (a.addNode(2))
            std::cout << "add node success" << std::endl;
        else
            std::cout << "add node fail" << std::endl;
        if (a.addEdge(1, 2, 3)) {
            std::cout << "add edge success" << std::endl;
        } else {
            std::cout << "add edge fail" << std::endl;
        }
        if (a.addEdge(1, 2, 3)) {
            std::cout << "add edge success" << std::endl;
        } else {
            std::cout << "add edge fail" << std::endl;
        }


    } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
    }

}