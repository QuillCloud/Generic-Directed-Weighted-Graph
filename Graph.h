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
    public:
        Graph() {};

        Graph(const Graph&);
        Graph(Graph&&);

    private:
        struct Node {
            struct Dst {
                E edge;
                std::weak_ptr<N> dst_name;
            };
            std::shared_ptr<N> node_name;
            std::vector<Dst> dst_list;
        };
        std::vector<Node> node_lsit;
    };

    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        node_lsit = g.node_lsit;
        std::cout << "copy construct" << std::endl;
    }

    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        node_lsit = std::move(g.node_lsit);
        std::cout << "move construct" << std::endl;
    }



};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
