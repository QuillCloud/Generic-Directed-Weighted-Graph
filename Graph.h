#ifndef GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
#define GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>


namespace gdwg {
    template <typename N, typename E> class Graph {
    public:
        // constructor
        Graph() {};
        Graph(const Graph&);
        Graph(Graph&&);
        // functions
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
        // nested class Node to present nodes in Graph
        class Node {
        public:
            // constructor
            Node(const N& val) : node_name{std::make_shared<N>(val)} {}
            // functions
            const N& get_node_name() const;
            std::shared_ptr<N> get_ptr() const;
            bool edge_in_node(const std::tuple<E, N>&) const;
            bool dst_in_node(const N&) const;
            void addEdge(const Node&, const E&);
            void replace(const N&);
            std::vector<std::tuple<E, N>> get_edge_list() const;
            void delete_invalid_edges();
            unsigned get_outdgree() const;
            void merge_edge(const Node&, const N&);
            void delete_edge(const std::tuple<E, N>&);

        private:
            // nested class Edge to present edge in each node
            class Edge {
            public:
                // constructor
                Edge(const Node& nd, const E& wt) : weight{wt} { dst_name = nd.get_ptr(); }
                // functions
                std::tuple<E, N> get_edge() const;
                bool is_valid() const;
                void change_dst(const Node&);
            private:
                // weak_ptr<N> point to shared_ptr<N> 'node_name', store the destination node name
                std::weak_ptr<N> dst_name;
                // store the edge weight
                E weight;
            };
            // store the node's name
            std::shared_ptr<N> node_name;
            // store the edges
            std::vector<Edge> edge_list;
        };
        //
        std::vector<Node> node_list;
        mutable typename std::vector<Node>::const_iterator my_it;
    };

    // copy constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        // add copy target nodes in new graph
        for (const Node& i : g.node_list) {
            addNode(i.get_node_name());
        }
        // add copy target edges in new graph
        for (const Node& i : g.node_list) {
            for (std::tuple<E, N> &j : i.get_edge_list()) {
                addEdge(i.get_node_name(), std::get<1>(j), std::get<0>(j));
            }
        }
    }

    // move constructor
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        node_list = std::move(g.node_list);
    }

    // copy assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {
        // clear the graph and add copy target nodes in graph
        node_list.clear();
        for (auto& i : g.node_list) {
            addNode(i.get_node_name());
        }
        // add copy target edges in graph
        for (auto& i : g.node_list) {
            for (std::tuple<E, N>& j : i.get_edge_list()) {
                addEdge(i.get_node_name(), std::get<1>(j), std::get<0>(j));
            }
        }
        return *this;
    }

    // move assignment
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& g) {
        node_list = std::move(g.node_list);
        return *this;
    }

    // add a new node with value val to the graph
    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        // check if node already in graph
        for (auto& i : node_list) {
            if (i.get_node_name() == val) {
                return false;
            }
        }
        // create a new node with value and add it to graph
        Node newNode(val);
        node_list.push_back(newNode);
        return true;
    }

    // add a new edge
    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        // find source node in graph
        auto srcNode= std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        // find destination node in graph
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        // if either node cannot be found in the graph, throw error
        if (srcNode == node_list.end() || dstNode == node_list.end()) {
            throw std::runtime_error("Source node or destination node not in the graph!");
        }
        // make a tuple as edge
        auto src_edge = std::make_tuple(w, dst);
        // if edge already in graph return false
        if (srcNode->edge_in_node(src_edge)) {
            return false;
        }
        // add edge to graph
        srcNode->addEdge(*dstNode, w);
        return true;
    }

    // replaces the original data, oldData, stored at this particular node by the replacement data, newData
    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        // find oldData node in graph
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node_name();
        });
        // if oldData node can not be found, throw an error
        if (oldNode == node_list.end()) {
            throw std::runtime_error("No node that contains value oldData can be found!");
        }
        // find newData node in graph
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node_name();
        });
        // if newData node already in graph, return false
        if (newNode != node_list.end()) {
            return false;
        }
        // replace the old data by new data
        oldNode->replace(newData);
        return true;
    }

    // replaces the data, oldData, stored at one node, denoted OldNode, with the data, newData
    // stored at another node, denoted NewNode, in the graph.
    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N& oldData, const N& newData) {
        // find oldData node in graph
        auto oldNode = std::find_if(node_list.begin(), node_list.end(), [&oldData] (const Node& nd) {
            return oldData == nd.get_node_name();
        });
        // find newData node in graph
        auto newNode = std::find_if(node_list.begin(), node_list.end(), [&newData] (const Node& nd) {
            return newData == nd.get_node_name();
        });
        // if either node can not be found, trow an error
        if (oldNode == node_list.end() || newNode == node_list.end()) {
            throw std::runtime_error("Either node cannot be found in the graph!");
        }
        // get the oldData node's edges list and add all edges to newData node
        auto egl = oldNode->get_edge_list();
        for (auto& i : egl) {
            addEdge(newData, std::get<1>(i), std::get<0>(i));
        }
        // for each node in graph, let newData node replace the oldData node for every edges
        for (auto& i : node_list) {
            i.merge_edge(*newNode, oldNode->get_node_name());
        }
        // delete the oldData node
        node_list.erase(oldNode);
    }

    // deletes a given node and all its associated incoming and outgoing edges
    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N& deleteData) noexcept {
        // find the given node
        auto deleteNode = std::find_if(node_list.begin(), node_list.end(), [&deleteData] (const Node& nd) {
            return deleteData == nd.get_node_name();
        });
        if (deleteNode != node_list.end()) {
            // delete node in graph
            node_list.erase(deleteNode);
            // for each node, delete the edges which destination node's weak pointer is expired
            for (auto& i : node_list) {
                i.delete_invalid_edges();
            }
        }
    }

    // deletes an edge from src to dst with weight w, only if the edge exists in the graph.
    template <typename N, typename E>
    void Graph<N, E>::deleteEdge(const N& src, const N& dst, const E& w) noexcept {
        // find source node in graph
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        // find destination node in graph
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        if (srcNode != node_list.end() && dstNode != node_list.end()) {
            // delete the edge in graph
            srcNode->delete_edge(std::make_tuple(w, dst));
        }
    }

    // remove all nodes and edges from the graph
    template <typename N, typename E>
    void Graph<N, E>::clear() noexcept {
        node_list.clear();
    }

    // returns true if a node with value val exists in the graph and false otherwise
    template <typename N, typename E>
    bool Graph<N, E>::isNode(const N& val) const {
        // find node in graph
        auto argNode = std::find_if(node_list.begin(), node_list.end(), [&val] (const Node& nd) {
            return val == nd.get_node_name();
        });
        // return true if find, false otherwise
        if (argNode != node_list.end()) {
            return true;
        }
        return false;
    }

    // returns true if the edge src → dst exists in the graph and false otherwise
    template <typename N, typename E>
    bool Graph<N, E>::isConnected(const N& src, const N& dst) const {
        // find source node in graph
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&src] (const Node& nd) {
            return src == nd.get_node_name();
        });
        // find destination node in graph
        auto dstNode = std::find_if(node_list.begin(), node_list.end(), [&dst] (const Node& nd) {
            return dst == nd.get_node_name();
        });
        // if either node not in graph, throw error
        if (srcNode == node_list.end() || dstNode == node_list.end()) {
            throw std::runtime_error("Either Source node or destination node is not in the graph!");
        }
        // check if source node contain the destination node
        return (srcNode->dst_in_node(dst));
    }

    // prints the data stored in all the nodes in the graph
    template <typename N, typename E>
    void Graph<N, E>::printNodes() const {
        // create a vector to contain the tuple (outdgree, node name)
        std::vector<std::tuple<unsigned , N>> temp;
        // for each node, get outdgree and name to make a tuple and add the tuple to vector
        for (auto& i : node_list) { 
            temp.push_back(std::make_tuple(i.get_outdgree(), i.get_node_name()));
        }
        // sort the vector, then go through vector and print each node's name
        std::sort(temp.begin(), temp.end());
        for (auto& i : temp) {
            std::cout << std::get<1>(i) << std::endl;
        }
    }

    // prints the outgoing edges of a given node, SrcNode, containing value val.
    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N& val) const {
        // find source node in graph
        auto srcNode = std::find_if(node_list.begin(), node_list.end(), [&val] (const Node& nd) {
            return val == nd.get_node_name();
        });
        // if source node can not be found throw error
        if (srcNode == node_list.end()) {
            throw std::runtime_error("Source Node does not exist in the graph!");
        }
        // get node's edges list (a vector)
        auto edge_list = srcNode->get_edge_list();
        std::cout << "Edges attached to Node " << srcNode->get_node_name() << std::endl;
        if (edge_list.empty()) {
            std::cout << "(null)" << std::endl;
        } else {
            // sort the edges list (a vector)
            // the element(edge) in list is tuple (weight, destination node name)
            std::sort(edge_list.begin(), edge_list.end());
            // print each edges in node
            for (auto& i : edge_list) {
                std::cout << std::get<1>(i) << " " << std::get<0>(i) << std::endl;
            }
        }
    }

    // sets an internal iterator
    template <typename N, typename E>
    void Graph<N, E>::begin() const {
        my_it = node_list.begin();
    }

    // returns true if the iterator goes past the last element of the sequence and false otherwise
    template <typename N, typename E>
    bool Graph<N, E>::end() const {
        return (my_it == node_list.end());
    }

    // moves the iterator to the next element of the sequence
    template <typename N, typename E>
    void Graph<N, E>::next() const {
        ++my_it;
    }

    // returns the value of the node pointed to by the iterator
    template <typename N, typename E>
    const N& Graph<N, E>::value() const {
        return(my_it->get_node_name());
    }

    // return the Node's name
    template <typename N, typename E>
    const N& Graph<N, E>::Node::get_node_name() const {
        return *node_name;
    }

    // return the shared pointer which point to Node's name
    template <typename N, typename E>
    std::shared_ptr<N> Graph<N, E>::Node::get_ptr() const {
        return node_name;
    }

    // check if one edge is in Node
    template <typename N, typename E>
    bool Graph<N, E>::Node::edge_in_node(const std::tuple<E, N>& tp) const {
        // go through edge list, if any edge same as argument edge, return true, otherwise return false
        for (auto& i : edge_list) {
            if (i.get_edge() == tp)
                return true;
        }
        return false;
    }

    // check if one destination node in Node
    template <typename N, typename E>
    bool Graph<N, E>::Node::dst_in_node(const N& val) const {
        // find edge that contain the destination node
        auto argDst = std::find_if(edge_list.begin(), edge_list.end(), [&val] (const Edge& ds) {
            return val == std::get<1>(ds.get_edge());
        });
        // return true if find, false otherwise
        if (argDst != edge_list.end()) {
            return true;
        }
        return false;
    }

    // add edge in class Node
    template <typename N, typename E>
    void Graph<N, E>::Node::addEdge(const Node& nd, const E& eg)  {
        // create a Edge and add tu edge list
        Edge d(nd, eg);
        edge_list.push_back(d);
    }

    // change the Node's name, the object which shared pointer point to
    template <typename N, typename E>
    void Graph<N, E>::Node::replace(const N& newName) {
        *node_name = newName;
    }

    // return edges list in Node, edges present as tuple (weight, destination node name)
    template <typename N, typename E>
    std::vector<std::tuple<E, N>> Graph<N, E>::Node::get_edge_list() const {
        // create a vector that store edge
        std::vector<std::tuple<E, N>> temp;
        // add each edge to vector and return the vector
        for (auto& i : edge_list) {
            temp.push_back(i.get_edge());
        }
        return temp;
    }

    // delete the edges which destination node's weak pointer is expired
    template <typename N, typename E>
    void Graph<N, E>::Node::delete_invalid_edges() {
        // create a new vector
        decltype(edge_list) temp;
        // add edges to new vector except the edge that destination node's weak pointer is expired
        for (auto& i : edge_list) {
            if (!i.is_valid()) {
                temp.push_back(i);
            }
        }
        // move new vector to edge list vector
        edge_list = std::move(temp);
    }

    // return outdgree of Node
    template <typename N, typename E>
    unsigned Graph<N, E>::Node::get_outdgree() const {
        return static_cast<unsigned >(edge_list.size());
    }

    // change edge in Node, let the destination node weak pointer point to a new Node
    template <typename N, typename E>
    void Graph<N, E>::Node::merge_edge(const Node& nd, const N& val) {
        // find the edge that contain the given destination node name
        for (auto& i : edge_list) {
            auto edge_tuple = i.get_edge();
            if (std::get<1>(edge_tuple) == val) {
                // let the destination node weak pointer point to given Node
                i.change_dst(nd);
            }
        }
    }

    // delete the edge in Node
    template <typename N, typename E>
    void Graph<N, E>::Node::delete_edge(const std::tuple<E, N>& val) {
        // find the given edge
        auto deleteEdge= std::find_if(edge_list.begin(), edge_list.end(), [&val] (const Edge& eg) {
            return val == eg.get_edge();
        });
        // if given edge is found, delete the edge in Node
        if (deleteEdge != edge_list.end()) {
            edge_list.erase(deleteEdge);
        }
    }

    // return the edge as a tuple (weight, destination node name)
    template <typename N, typename E>
    std::tuple<E, N> Graph<N, E>::Node::Edge::get_edge() const {
        auto ds = dst_name.lock();
        return std::make_tuple(weight, *ds);
    }

    // check if the destination node weak pointer is expired
    template <typename N, typename E>
    bool Graph<N, E>::Node::Edge::is_valid() const {
        return (dst_name.expired());
    }

    // let the destination node weak pointer point to given Node
    template <typename N, typename E>
    void Graph<N, E>::Node::Edge::change_dst(const Node& nd) {
        dst_name = nd.get_ptr();
    }
};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
