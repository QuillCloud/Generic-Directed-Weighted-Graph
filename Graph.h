//
// Created by 张云翮 on 09/09/2017.
//

#ifndef GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
#define GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H

#include <iostream>
#include <vector>
#include <tuple>


namespace gdwg {
    template <typename N, typename E> class Graph {
    public:
        Graph();
        Graph(const Graph&);
        Graph(Graph&&);
        ~Graph();
        bool addNode(const N&);
        bool addEdge(const N&, const N&, const E&);
        bool replace(const N&, const N&);

    private:
        //
        std::unique_ptr<std::vector<N>> node_list
                = std::make_unique<std::vector<N>>(std::vector<N>());
        //
        std::unique_ptr<std::vector<std::tuple<N, N, E>>> edge_list
                = std::make_unique<std::vector<std::tuple<N, N, E>>>(std::vector<std::tuple<N, N, E>>());
    };

    // Graph.tem
    // constructor
    template <typename N, typename E>
    Graph<N, E>::Graph() {
        std::cout << "constructor" << std::endl;
    }

    // copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph<N, E>& graph) {
        *node_list = *graph.node_list;
        *edge_list = *graph.edge_list;
        std::cout << "copy constructor" << std::endl;
    }

    // move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph<N, E>&& graph) {
        *node_list = std::move(*graph.node_list);
        *edge_list = std::move(*graph.edge_list);
        graph.node_list = nullptr;
        graph.edge_list = nullptr;
        std::cout << "move constructor" << std::endl;
    }

    // destructor
    template <typename N, typename E>
    Graph<N, E>::~Graph() {
        std::cout << "destructor" << std::endl;
    }

    // addNode
    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        if (std::find(node_list->begin(), node_list->end(), val) != node_list->end()) {
            std::cout << "add node \"" << val << "\" fail" << std::endl;
            return false;
        } else {
            node_list->push_back(val);
            std::cout << "add node \"" << val << "\" success" << std::endl;
            return true;
        }
    }

    // addEdge
    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        if (std::find(node_list->begin(), node_list->end(), src) == node_list->end()) {
            throw std::runtime_error("src is not in the graph");
        } else if (std::find(node_list->begin(), node_list->end(), dst) == node_list->end()) {
            throw std::runtime_error("dst is not in the graph");
        } else {
            auto tp = std::make_tuple(src, dst, w);
            if (std::find(edge_list->begin(), edge_list->end(), tp) != edge_list->end()) {
                std::cout << "add edge (" << src << ", " << dst << ", " << w << ") fail" << std::endl;
                return false;
            } else {
                edge_list->push_back(tp);
                std::cout << "add edge (" << src << ", " << dst << ", " << w << ") success" << std::endl;
                return true;
            }
        }
    }

    // replace
    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        if (std::find(node_list->begin(), node_list->end(), oldData) == node_list->end()) {
            throw std::runtime_error("no node that contains value oldData can be found");
        } else {
            if (std::find(node_list->begin(), node_list->end(), oldData) == node_list->end()) {
                return false;
            } else {
                std::replace(node_list->begin(), node_list->end(), oldData, newData);
                for (auto& edge : *edge_list) {
                    std::cout << std::get<0>(edge) << std::endl;
                }
            }
        }
    }



};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
