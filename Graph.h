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
        Graph& operator=(const Graph&);
        Graph& operator=(Graph&&);
        bool addNode(const N&);
        bool addEdge(const N&, const N&, const E&);
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

    private:
        class Node {
        public:
            Node(const N& val) : node_name{std::make_shared<N>(val)} {}
            const N& get_node() const { return *node_name; }
            std::weak_ptr<N> get_ptr() const { return node_name; }
            bool edge_in_node(const std::tuple<N, E>& tp) const {
                for (auto& i : dst_list) {
                    if (i.get_edge() == tp)
                        return true;
                }
                return false;
            }
            void addDst(const Node& nd, const E& eg) {
                Dst d(nd, eg);
                dst_list.push_back(d);
            }

        private:
            class Dst {
            public:
                Dst(const Node& nd, const E& eg) : edge{eg} {
                    dst_name = nd.get_ptr();
                }
                std::tuple<N, E> get_edge() const;
            private:
                std::weak_ptr<N> dst_name;
                E edge;
            };
            std::shared_ptr<N> node_name;
            std::vector<Dst> dst_list;
        };
        std::vector<Node> node_lsit;
    };

    // copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        node_lsit = g.node_lsit;
        std::cout << "copy construct" << std::endl;
    }

    // move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        node_lsit = std::move(g.node_lsit);
        std::cout << "move construct" << std::endl;
    }

    // copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {
        node_lsit = g.node_lsit;
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    // move assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& g) {
        node_lsit = std::move(g.node_lsit);
        std::cout << "move assignment" << std::endl;
        return *this;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        for (const auto& i : node_lsit) {
            if (i.get_node() == val) {
                return false;
            }
        }
        Node newNode(val);
        node_lsit.push_back(newNode);
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        auto srcNode= std::find_if(node_lsit.begin(), node_lsit.end(), [&src] (const Node& nd) {
            return src == nd.get_node();
        });
        auto dstNode = std::find_if(node_lsit.begin(), node_lsit.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node();
        });
        if (srcNode != node_lsit.end() && dstNode != node_lsit.end()) {
            auto src_edge = std::make_tuple(dst, w);
            if (srcNode->edge_in_node(src_edge))
                return false;
            srcNode->addDst(*dstNode, w);
            return true;
        } else {
            throw std::runtime_error("Source node or destination node not in the graph!");
        }

    }

    template <typename N, typename E>
    std::tuple<N, E> Graph<N, E>::Node::Dst::get_edge() const {
        auto ds = dst_name.lock();
        return std::make_tuple(*ds, edge);
    }


};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
