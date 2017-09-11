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
        Graph(const Graph&);
        Graph(Graph&&);
        ~Graph();
        Graph& operator=(const Graph&);
        Graph& operator=(Graph&&);
        bool addNode(const N&);
        bool addEdge(const N&, const N&, const E&);
        /*
        bool replace(const N&, const N&);
        void mergeReplace(const N&, const N&);
        void deleteNode(const N&) noexcept;
        void deleteEdge(const N&, const N&, const E&) noexcept;
        void clear() noexcept;
        bool isNode(const N&) const;
        bool isConnected(const N&, const N&) const;
        void printNodes() const;
        void printEdges(const N&) const;
        void begin() const;
        bool end() const;
        void next() const;
        const N& value() const;

        void print();
         */

    private:
        //
        std::vector<std::shared_ptr<N>> node_list = std::vector<std::shared_ptr<N>>();
        //
        std::vector<std::weak_ptr<std::tuple<N, N, E>>> edge_list = std::vector<std::weak_ptr<std::tuple<N, N, E>>>();
        mutable typename std::vector<N>::iterator my_it;
    };

// Graph.tem
// constructor
    template <typename N, typename E>
    Graph<N, E>::Graph() {
        //std::cout << "constructor" << std::endl;
    }
/*
// copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph<N, E>& graph) {
        node_list = graph.node_list;
        edge_list = graph.edge_list;
        //std::cout << "copy constructor" << std::endl;
    }

// move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph<N, E>&& graph) {
        node_list = std::move(graph.node_list);
        edge_list = std::move(graph.edge_list);
        //std::cout << "move constructor" << std::endl;
    }

// destructor
    template <typename N, typename E>
    Graph<N, E>::~Graph() {
        //std::cout << "destructor" << std::endl;
    }

// copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& graph) {
        node_list = graph.node_list;
        edge_list = graph.edge_list;
        //std::cout << "copy assignment" << std::endl;
        return *this;
    }

// move assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& graph) {
        node_list = std::move(graph.node_list);
        edge_list = std::move(graph.edge_list);
        //std::cout << "move assignment" << std::endl;
        return *this;
    }

// addNode
    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        // return false if node already in graph
        std::shared_ptr<N> val_ptr = std::make_shared<N>(val);
        if (std::find(node_list.begin(), node_list.end(), val_ptr) != node_list.end()) {
            //std::cout << "add node \"" << val << "\" fail" << std::endl;
            return false;
        } else {
            // add node to graph
            node_list.push_back(val_ptr);
            //std::cout << "add node \"" << val << "\" success" << std::endl;
            return true;
        }
    }

// addEdge
    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        // throw runtime_error if source node or destination node is not in the graph
        if (std::find(node_list.begin(), node_list.end(), std::make_shared<N>(src)) == node_list.end()) {
            throw std::runtime_error("Source node is not in the graph!");
        } else if (std::find(node_list.begin(), node_list->end(), dst) == node_list->end()) {
            throw std::runtime_error("Destination node  is not in the graph!");
        } else {
            // create the the edge that will be added
            auto tp = std::make_tuple(src, dst, w);
            // return false if edge already in graph
            if (std::find(edge_list->begin(), edge_list->end(), tp) != edge_list->end()) {
                //std::cout << "add edge (" << src << ", " << dst << ", " << w << ") fail" << std::endl;
                return false;
            } else {
                // add edge to graph
                edge_list->push_back(tp);
                //std::cout << "add edge (" << src << ", " << dst << ", " << w << ") success" << std::endl;
                return true;
            }
        }
    }
/*
// replace
    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        // throw runtime_error if graph do not contain old data
        if (std::find(node_list->begin(), node_list->end(), oldData) == node_list->end()) {
            throw std::runtime_error("No node that contains value oldData can be found!");
        } else {
            // return false if new data already in graph
            if (std::find(node_list->begin(), node_list->end(), newData) != node_list->end()) {
                //std::cout << "replace node \"" << oldData << "\" with \"" << newData << "\" fail" << std::endl;
                return false;
            } else {
                // replace the old data with new data in node list
                std::replace(node_list->begin(), node_list->end(), oldData, newData);
                // replace the old data with new data in edge list
                for (auto& edge : *edge_list) {
                    if (std::get<0>(edge) == oldData) {
                        std::get<0>(edge) = newData;
                    }
                    if (std::get<1>(edge) == oldData) {
                        std::get<1>(edge) = newData;
                    }
                }
                //std::cout << "replace node \"" << oldData << "\" with \"" << newData << "\" success" << std::endl;
                return true;
            }
        }
    }

// merge replace
    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N& oldData, const N& newData) {
        if (std::find(node_list->begin(), node_list->end(), oldData) == node_list->end()) {
            throw std::runtime_error("Old node is not in the graph!");
        } else if (std::find(node_list->begin(), node_list->end(), newData) == node_list->end()) {
            throw std::runtime_error("New node is not in the graph!");
        } else {
            // remove old node from node list
            node_list->erase(std::find(node_list->begin(), node_list->end(), oldData));
            // replace the old data with new data in edge list
            for (auto& edge: *edge_list) {
                if (std::get<0>(edge) == oldData) {
                    std::get<0>(edge) = newData;
                }
                if (std::get<1>(edge) == oldData) {
                    std::get<1>(edge) = newData;
                }
            }
            // sort the edge list and merge duplicated edges
            std::sort(edge_list->begin(), edge_list->end());
            edge_list->erase(std::unique(edge_list->begin(), edge_list->end()), edge_list->end());
            //std::cout << "merge replace node \"" << oldData << "\" with \"" << newData << "\" success" << std::endl;
        }
    }

// delete node
    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N& val) noexcept {
        // check if node is in graph
        if (std::find(node_list->begin(), node_list->end(), val) != node_list->end()) {
            // remove node from node list
            node_list->erase(std::find(node_list->begin(), node_list->end(), val));
            // go through edge list, drop the edge that contains the deleted node
            std::vector<std::tuple<N, N, E>> temp = std::vector<std::tuple<N, N, E>>();
            while (!edge_list->empty()) {
                if (std::get<0>(edge_list->back()) != val && std::get<1>(edge_list->back()) != val) {
                    temp.push_back(edge_list->back());
                }
                edge_list->pop_back();
            }
            for (const auto& edge : temp) {
                edge_list->push_back(edge);
            }
        }
    }

// delete edge
    template <typename N, typename E>
    void Graph<N, E>::deleteEdge(const N& src, const N& dst, const E& w) noexcept {
        auto tp = std::make_tuple(src, dst, w);
        if (std::find(edge_list->begin(), edge_list->end(), tp) != edge_list->end())
            edge_list->erase(std::find(edge_list->begin(), edge_list->end(), tp));
    }

// clear
    template <typename N, typename E>
    void Graph<N, E>::clear() noexcept {
        node_list->clear();
        edge_list->clear();
    }

    template <typename N, typename E>
    bool Graph<N, E>::isNode(const N& val) const {
        if (std::find(node_list->begin(), node_list->end(), val) != node_list->end()) {
            return true;
        }
        return false;
    }

// is connected
    template <typename N, typename E>
    bool Graph<N, E>::isConnected(const N& src, const N& dst) const {
        // throw runtime_error if source node or destination node is not in the graph
        if (std::find(node_list->begin(), node_list->end(), src) == node_list->end()) {
            throw std::runtime_error("Source node is not in the graph!");
        } else if (std::find(node_list->begin(), node_list->end(), dst) == node_list->end()) {
            throw std::runtime_error("Destination node  is not in the graph!");
        } else {
            // if any edge src → dst exists in graph, return true, otherwise return false
            for (const auto& edge : *edge_list) {
                if (std::get<0>(edge) == src && std::get<1>(edge) == dst) {
                    return true;
                }
            }
            return false;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::printNodes() const {
        std::map<N, int> m = std::map<N, int>();
        for (const auto& node : *node_list) {
            m.insert(std::make_pair(node, 0));
        }
        for (const auto& edge : *edge_list) {
            auto it = m.find(std::get<0>(edge));
            if (it != m.end()) {
                it->second += 1;
            }
        }
        std::vector<std::tuple<int, N>> v = std::vector<std::tuple<int, N>>();
        for (typename std::map<N, int>::iterator it = m.begin(); it != m.end(); ++it) {
            v.push_back(std::make_tuple(it->second, it-> first));
        }
        std::sort(v.begin(), v.end());
        for (const auto& i : v) {
            std::cout << std::get<1>(i) << std::endl;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N& val) const {
        if (std::find(node_list->begin(), node_list->end(), val) == node_list->end()) {
            throw std::runtime_error("Source node is not in graph!");
        } else {
            std::vector<std::tuple<E, N>> val_list = std::vector<std::tuple<E, N>>();
            for (const auto &edge : *edge_list) {
                if (std::get<0>(edge) == val) {
                    val_list.push_back(std::make_tuple(std::get<2>(edge), std::get<1>(edge)));
                }
            }
            std::cout << "Edges attached to Node " << val << std::endl;
            if (val_list.empty()) {
                std::cout << "(null)" << std::endl;
            } else {
                std::sort(val_list.begin(), val_list.end());
                for (const auto& i : val_list) {
                    std::cout << std::get<1>(i) << " " << std::get<0>(i) << std::endl;
                }
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::begin() const {
        my_it = node_list->begin();
    }

    template <typename N, typename E>
    bool Graph<N, E>::end() const {
        return (my_it == node_list->end());
    }

    template <typename N, typename E>
    void Graph<N, E>::next() const {
        ++my_it;
    }

    template <typename N, typename E>
    const N& Graph<N, E>::value() const {
        return *my_it;
    }



    template <typename N, typename E>
    void Graph<N, E>::print() {
        for (auto& node : *node_list) {
            std::cout << node << std::endl;
        }
        for (auto& edge : *edge_list) {
            std::cout << std::get<0>(edge) << " " << std::get<1>(edge) <<  " " << std::get<2>(edge) << std::endl;
        }
    }
    */
};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
