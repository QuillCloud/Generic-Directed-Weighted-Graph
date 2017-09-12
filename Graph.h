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
            const N& get_node_name() const { return *node_name; }
            std::shared_ptr<N> get_ptr() const { return node_name; }
            bool edge_in_node(const std::tuple<E, N>&) const;
            bool dst_in_node(const N&) const;
            void addDst(const Node&, const E&);
            void replace(const N&);
            std::vector<std::tuple<E, N>> get_edge_list() const;
            void delete_invalid_edges();
            unsigned get_outdgree() const;
            void merge_dst(const Node&, const N&);
            void delete_edge(const std::tuple<E, N>&);

        private:
            class Dst {
            public:
                Dst(const Node& nd, const E& eg) : edge{eg} { dst_name = nd.get_ptr(); }
                std::tuple<E, N> get_edge() const;
                bool is_valid() const;
                void change_dst(const Node& nd) { dst_name = nd.get_ptr(); }
            private:
                std::weak_ptr<N> dst_name;
                E edge;
            };
            std::shared_ptr<N> node_name;
            std::vector<Dst> dst_list;
        };
        std::vector<Node> node_list;
        mutable typename std::vector<Node>::const_iterator my_it;
    };

    // copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        for (const Node& i : g.node_list) {
            addNode(i.get_node_name());
        }
        for (const Node& i : g.node_list) {
            for (std::tuple<E, N>& j : i.get_edge_list()) {
                addEdge(i.get_node_name(), std::get<1>(j), std::get<0>(j));
            }
        }
        //std::cout << "copy construct" << std::endl;
    }

    // move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        node_list = std::move(g.node_list);
        //std::cout << "move construct" << std::endl;
    }

    // copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {
        node_list.clear();
        for (auto& i : g.node_list) {
            addNode(i.get_node_name());
        }
        for (auto& i : g.node_list) {
            for (std::tuple<E, N>& j : i.get_edge_list()) {
                addEdge(i.get_node_name(), std::get<1>(j), std::get<0>(j));
            }
        }
        //std::cout << "copy assignment" << std::endl;
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
        for (auto& i : node_list) {
            if (i.get_node_name() == val) {
                //std::cout << "add Node \"" << val << "\" fail" << std::endl;
                return false;
            }
        }
        Node newNode(val);
        node_list.push_back(newNode);
        //std::cout << "add Node \"" << val << "\" success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        auto srcNode= std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        if (srcNode == node_list.end() || dstNode == node_list.end()) {
            throw std::runtime_error("Source node or destination node not in the graph!");
        }
        auto src_edge = std::make_tuple(w, dst);
        if (srcNode->edge_in_node(src_edge)) {
            //std::cout << "add Edge (" << src << ", " << dst <<  ", " << w << ") fail" << std::endl;
            return false;
        }
        srcNode->addDst(*dstNode, w);
        //std::cout << "add Edge (" << src << ", " << dst <<  ", " << w << ") success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node_name();
        });
        if (oldNode == node_list.end()) {
            throw std::runtime_error("No node that contains value oldData can be found!");
        }
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node_name();
        });
        if (newNode != node_list.end()) {
            //std::cout << "replace Node \"" << oldData << "\" to \"" << newData << "\" fail" << std::endl;
            return false;
        }
        oldNode->replace(newData);
        //std::cout << "replace Node \"" << oldData << "\" to \"" << newData << "\" success" << std::endl;
        return true;
    }

    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N& oldData, const N& newData) {
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node_name();
        });
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node_name();
        });
        if (oldNode == node_list.end() || newNode == node_list.end()) {
            throw std::runtime_error("Either node cannot be found in the graph!");
        }
        auto egl = oldNode->get_edge_list();
        for (auto& i : egl) {
            addEdge(newData, std::get<1>(i), std::get<0>(i));
        }
        for (auto& i : node_list) {
            i.merge_dst(*newNode, oldNode->get_node_name());
        }
        node_list.erase(oldNode);
    }

    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N& deleteData) noexcept {
        auto deleteNode = std::find_if(node_list.begin(), node_list.end(), [&deleteData] (const Node& nd) {
            return deleteData == nd.get_node_name();
        });
        if (deleteNode != node_list.end()) {
            node_list.erase(deleteNode);
            for (auto& i : node_list) {
                i.delete_invalid_edges();
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::deleteEdge(const N& src, const N& dst, const E& w) noexcept {
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        if (srcNode != node_list.end() && dstNode != node_list.end()) {
            srcNode->delete_edge(std::make_tuple(w, dst));
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::clear() noexcept {
        node_list.clear();
    }

    template <typename N, typename E>
    bool Graph<N, E>::isNode(const N& val) const {
        auto argNode = std::find_if(node_list.begin(), node_list.end(), [&val] (const Node& nd) {
            return val == nd.get_node_name();
        });
        if (argNode != node_list.end()) {
            return true;
        }
        return false;
    }

    template <typename N, typename E>
    bool Graph<N, E>::isConnected(const N& src, const N& dst) const {
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        if (srcNode == node_list.end() || dstNode == node_list.end()) {
            throw std::runtime_error("Either Source node or destination node is not in the graph!");
        }
        return (srcNode->dst_in_node(dst));
    }

    template <typename N, typename E>
    void Graph<N, E>::printNodes() const {
        std::vector<std::tuple<unsigned , N>> temp;
        for (auto& i : node_list) { 
            temp.push_back(std::make_tuple(i.get_outdgree(), i.get_node_name()));
        }
        std::sort(temp.begin(), temp.end());
        for (auto& i : temp) {
            std::cout << std::get<1>(i) << std::endl;
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N& val) const {
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&val] (const Node& nd) {
            return val == nd.get_node_name();
        });
        if (srcNode == node_list.end()) {
            throw std::runtime_error("Source Node does not exist in the graph!");
        }
        auto edge_list = srcNode->get_edge_list();
        std::cout << "Edges attached to Node " << srcNode->get_node_name() << std::endl;
        if (edge_list.empty()) {
            std::cout << "(null)" << std::endl;
        } else {
            std::sort(edge_list.begin(), edge_list.end());
            for (auto& i : edge_list) {
                std::cout << std::get<1>(i) << " " << std::get<0>(i) << std::endl;
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::begin() const {
        my_it = node_list.begin();
    }

    template <typename N, typename E>
    bool Graph<N, E>::end() const {
        return (my_it == node_list.end());
    }

    template <typename N, typename E>
    void Graph<N, E>::next() const {
        ++my_it;
    }

    template <typename N, typename E>
    const N& Graph<N, E>::value() const {
        return(my_it->get_node_name());
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::addDst(const Node& nd, const E& eg)  {
        Dst d(nd, eg);
        dst_list.push_back(d);
    }

    template <typename N, typename E>
    bool Graph<N, E>::Node::edge_in_node(const std::tuple<E, N>& tp) const {
        for (auto& i : dst_list) {
            if (i.get_edge() == tp)
                return true;
        }
        return false;
    }

    template <typename N, typename E>
    bool Graph<N, E>::Node::dst_in_node(const N& val) const {
        auto argDst = std::find_if(dst_list.begin(), dst_list.end(), [&val] (const Dst& ds) {
            return val == std::get<1>(ds.get_edge());
        });
        if (argDst != dst_list.end()) {
            return true;
        }
        return false;
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::replace(const N& newName) {
        *node_name = newName;
    }

    template <typename N, typename E>
    std::vector<std::tuple<E, N>> Graph<N, E>::Node::get_edge_list() const {
        std::vector<std::tuple<E, N>> temp;
        for (auto& i : dst_list) {
            temp.push_back(i.get_edge());
        }
        return temp;
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::delete_invalid_edges() {
        decltype(dst_list) temp;
        for (auto& i : dst_list) {
            if (!i.is_valid()) {
                temp.push_back(i);
            }
        }
        dst_list = std::move(temp);
    }

    template <typename N, typename E>
    unsigned Graph<N, E>::Node::get_outdgree() const {
        return static_cast<unsigned >(dst_list.size());
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::merge_dst(const Node& nd, const N& val) {
        for (auto& i : dst_list) {
            auto edge_tuple = i.get_edge();
            if (std::get<1>(edge_tuple) == val) {
                i.change_dst(nd);
            }
        }
    }

    template <typename N, typename E>
    void Graph<N, E>::Node::delete_edge(const std::tuple<E, N>& eg) {
        auto deleteEdge= std::find_if(dst_list.begin(), dst_list.end(), [&eg] (const Dst& ds) {
            return eg == ds.get_edge();
        });
        if (deleteEdge != dst_list.end()) {
            dst_list.erase(deleteEdge);
        }
    }

    template <typename N, typename E>
    std::tuple<E, N> Graph<N, E>::Node::Dst::get_edge() const {
        auto ds = dst_name.lock();
        return std::make_tuple(edge, *ds);
    }

    template <typename N, typename E>
    bool Graph<N, E>::Node::Dst::is_valid() const {
        return (dst_name.lock() == nullptr);
    }
};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
