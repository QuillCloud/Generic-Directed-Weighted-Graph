// test for printNodes()

#include "Graph.h"

int main() {

    // make a graph
    gdwg::Graph<unsigned int, int> a;
    gdwg::Graph<unsigned int, int> b(a);
    gdwg::Graph<unsigned int, int> c(std::move(a));
    gdwg::Graph<unsigned int, int> d;
    d = b;
    a = std::move(d);

}