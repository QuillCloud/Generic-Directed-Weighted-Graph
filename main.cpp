// test for printNodes()

#include "Graph.h"

int main() {

    // make a graph
    gdwg::Graph<unsigned int, int> g;
    gdwg::Graph<unsigned int, int> l(g);
    gdwg::Graph<unsigned int, int> j(std::move(g));

}