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
        Graph& operator=(const Graph&);
        Graph& operator=(Graph&&);
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

    // copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& graph) {
        *node_list = *graph.node_list;
        *edge_list = *graph.edge_list;
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    // move assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& graph) {
        *node_list = std::move(*graph.node_list);
        *edge_list = std::move(*graph.edge_list);
        graph.node_list = nullptr;
        graph.edge_list = nullptr;
        std::cout << "move assignment" << std::endl;
        return *this;
    }

    // addNode
    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        // return false if node already in graph
        if (std::find(node_list->begin(), node_list->end(), val) != node_list->end()) {
            std::cout << "add node \"" << val << "\" fail" << std::endl;
            return false;
        } else {
            // add node to graph
            node_list->push_back(val);
            std::cout << "add node \"" << val << "\" success" << std::endl;
            return true;
        }
    }

    // addEdge
    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        // throw runtime_error if src or dst is not in the graph
        if (std::find(node_list->begin(), node_list->end(), src) == node_list->end()) {
            throw std::runtime_error("src is not in the graph");
        } else if (std::find(node_list->begin(), node_list->end(), dst) == node_list->end()) {
            throw std::runtime_error("dst is not in the graph");
        } else {
            // create the the edge that will be added
            auto tp = std::make_tuple(src, dst, w);
            // return false if edge already in graph
            if (std::find(edge_list->begin(), edge_list->end(), tp) != edge_list->end()) {
                std::cout << "add edge (" << src << ", " << dst << ", " << w << ") fail" << std::endl;
                return false;
            } else {
                // add edge to graph
                edge_list->push_back(tp);
                std::cout << "add edge (" << src << ", " << dst << ", " << w << ") success" << std::endl;
                return true;
            }
        }
    }

    // replace
    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        // throw runtime_error if graph do not contain old data
        if (std::find(node_list->begin(), node_list->end(), oldData) == node_list->end()) {
            throw std::runtime_error("no node that contains value oldData can be found");
        } else {
            // return false if new data already in graph
            if (std::find(node_list->begin(), node_list->end(), newData) != node_list->end()) {
                std::cout << "replace node \"" << oldData << "\" with \"" << newData << "\" fail" << std::endl;
                return false;
            } else {
                // replace the old data with new data in node list
                std::replace(node_list->begin(), node_list->end(), oldData, newData);
                // replace the old data with new data in edge list
                for (auto& edge : *edge_list) {
                    if (std::get<0>(edge) == oldData) {
                        std::get<0>(edge) = newData;
                    } else if (std::get<1>(edge) == oldData) {
                        std::get<1>(edge) = newData;
                    }
                }
                std::cout << "replace node \"" << oldData << "\" with \"" << newData << "\" success" << std::endl;
                return true;
            }
        }
    }
};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
