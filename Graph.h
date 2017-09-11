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
#include <algorithm>


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
            bool edge_in_node(const std::tuple<N, E>&) const;
            void addDst(const Node&, const E&);
            void replace(const N&);
            std::vector<std::tuple<N, E>> get_edge_list() const;
            void clean_up_edges();

        private:
            class Dst {
            public:
                Dst(const Node& nd, const E& eg) : edge{eg} { dst_name = nd.get_ptr(); }
                std::tuple<N, E> get_edge() const;
                bool is_nullptr() const;
            private:
                std::weak_ptr<N> dst_name;
                E edge;
            };
            std::shared_ptr<N> node_name;
            std::vector<Dst> dst_list;
        };
        std::vector<Node> node_list;
    };

    // copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        node_list = g.node_list;
        std::cout << "copy construct" << std::endl;
    }

    // move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        node_list = std::move(g.node_list);
        std::cout << "move construct" << std::endl;
    }

    // copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {
        node_list = g.node_list;
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    // move assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& g) {
        node_list = std::move(g.node_list);
        std::cout << "move assignment" << std::endl;
        return *this;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        for (const auto& i : node_list) {
            if (i.get_node() == val) {
                std::cout << "add Node \"" << val << "\" fail" << std::endl;
                return false;
            }
        }
        Node newNode(val);
        node_list.push_back(newNode);
        std::cout << "add Node \"" << val << "\" success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        auto srcNode= std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node();
        });
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node();
        });
        if (srcNode == node_list.end() || dstNode == node_list.end()) {
            throw std::runtime_error("Source node or destination node not in the graph!");
        }
        auto src_edge = std::make_tuple(dst, w);
        if (srcNode->edge_in_node(src_edge)) {
            std::cout << "add Edge (" << src << ", " << dst <<  ", " << w << ") fail" << std::endl;
            return false;
        }
        srcNode->addDst(*dstNode, w);
        std::cout << "add Edge (" << src << ", " << dst <<  ", " << w << ") success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node();
        });
        if (oldNode == node_list.end()) {
            throw std::runtime_error("No node that contains value oldData can be found!");
        }
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node();
        });
        if (newNode != node_list.end()) {
            std::cout << "replace Node \"" << oldData << "\" to \"" << newData << "\" fail" << std::endl;
            return false;
        }
        oldNode->replace(newData);
        std::cout << "replace Node \"" << oldData << "\" to \"" << newData << "\" success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N& oldData, const N& newData) {
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node();
        });
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node();
        });
        if (oldNode == node_list.end() || newNode == node_list.end()) {
            throw std::runtime_error("Either node cannot be found in the graph!");
        }
        auto egl = oldNode->get_edge_list();
        for (auto& i : egl) {
            addEdge(newData, std::get<0>(i), std::get<1>(i));
        }
        node_list.erase(oldNode);
    }

    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N& deleteData) noexcept {
        auto deleteNode = std::find_if(node_list.begin(), node_list.end(), [&deleteData] (const Node& nd) {
            return deleteData == nd.get_node();
        });
        if (deleteNode != node_list.end()) {
            node_list.erase(deleteNode);
            for (auto& i : node_list) {
                i.clean_up_edges();
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::addDst(const Node &nd, const E &eg)  {
        std::cout << dst_list.size() << std::endl;
        Dst d(nd, eg);
        dst_list.push_back(d);
    }

    template <typename N, typename E>
    bool Graph<N, E>::Node::edge_in_node(const std::tuple<N, E>& tp) const {
        for (auto& i : dst_list) {
            if (i.get_edge() == tp)
                return true;
        }
        return false;
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::replace(const N& newName) {
        *node_name = newName;
    }

    template <typename N, typename E>
    std::vector<std::tuple<N, E>> Graph<N, E>::Node::get_edge_list() const {
        auto temp = std::vector<std::tuple<N, E>> ();
        for (auto& i : dst_list) {
            temp.push_back(i.get_edge());
        }
        return temp;
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::clean_up_edges() {
        decltype(dst_list) temp;
        for (auto& i : dst_list) {
            if (!i.is_nullptr()) {
                temp.push_back(i);
            }
        }
        dst_list = std::move(temp);
    }

    template <typename N, typename E>
    std::tuple<N, E> Graph<N, E>::Node::Dst::get_edge() const {
        auto ds = dst_name.lock();
        return std::make_tuple(*ds, edge);
    }

    template <typename N, typename E>
    bool Graph<N, E>::Node::Dst::is_nullptr() const {
        return (dst_name.lock() == nullptr);
    }


};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
