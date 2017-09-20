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
            void add_edge(const Node&, const E&);
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
                // store the edge weight of this edge
                E weight;
            };
            // store the node's name
            std::shared_ptr<N> node_name;
            // store the edges of this Node
            std::vector<Edge> edge_list;
        };
        //vector for storing the Node
        std::vector<Node> node_list;
        // iterator for providing a fake iterator for enumerating all the node values in a graph
        mutable typename std::vector<Node>::const_iterator my_it;
    };

    #include "Graph.tem"

};

#endif //GENERIC_DIRECTED_WEIGHTED_GRAPH_GRAPH_H
