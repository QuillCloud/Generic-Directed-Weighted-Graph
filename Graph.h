//
// Created by 张云翮 on 09/09/2017.
//

#ifndef GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
#define GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H

#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <memory>


namespace gdwg {
    template <typename N, typename E> class Graph {
    public:public:
        Graph();

    private:
        //
        std::vector<std::shared_ptr<N>> node_list = std::vector<std::shared_ptr<N>>();
        //
       	std::vector<std::weak_ptr<std::tuple<N, N, E>>> edge_list;
        mutable typename std::vector<N>::iterator my_it;
    };

// Graph.tem
// constructor
    template <typename N, typename E>
    Graph<N, E>::Graph() {
        std::cout << "constructor" << std::endl;
    }

};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
