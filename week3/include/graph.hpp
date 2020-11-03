#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#define itr_range(cont) std::begin(cont), std::end(cont) /// handy macro

class Vertex;
class Edge;

using vertex_id = int;
using vertex_value_t = int;
using edge_weight_t = int;

using vertex_ptr = std::shared_ptr<Vertex>;
using vertex_pref = const vertex_ptr &;
using vertex_wptr = std::weak_ptr<Vertex>;

using edge_ptr = std::unique_ptr<Edge>;
using edge_pref = const edge_ptr &;

/// \brief representation of an edge, tracking from/to vertices and the weight
/// between them
///
/// since the this class depends on the Vertex class, we must use pointers, but
/// smart pointers are better. this class keeps track of vertices using weak
/// pointers to avoid memory overhead
class Edge {
    vertex_wptr _source; ///< vertex we're going from
    vertex_wptr _sink;   ///< vertex we're going to
    edge_weight_t _wei;  ///< the weight between the two vertices

  public:
    Edge() = delete;             ///< cannot be constructed by default
    Edge(const Edge &) = delete; ///< cannot be constructed by copy
    Edge(Edge &&other) = delete; ///< nor can it be constructed by a move

    /// \brief creating an edge between two vertices, both should exist
    /// beforehand. throws if either is nullptr
    ///
    /// \param from pointer to the vertex we're coming from
    /// \param to pointer to the vertex we're going to
    /// \param wei the weight og the ride
    Edge(vertex_pref from, vertex_pref to, const edge_weight_t &wei);

    Edge &operator=(const Edge &other) = delete; ///< cannot be copied
    Edge &operator=(Edge &&other) = delete;      ///< cannot be moved

    /// \brief the vertex this edge pointing from
    ///
    /// \return a shared pointer from the locked weak pointer
    vertex_ptr from() const;

    /// \brief the vertex this edge pointing to
    ///
    /// \return a shared pointer from the locked weak pointer
    vertex_ptr to() const;

    const edge_weight_t &weight() const;   ///< accessor to the weight
    void weight(const edge_weight_t &wei); ///< mutator of the weight
};

/// \brief Vertex represenation using adjacensy list. Each vertex keeps track of
/// which out edges it has as well as its id and value
///
/// the inhiritance from enable_shared_from_this is to avoid provide a shared
/// pointer to the current insatnce double free errors.
///
/// check this SO question: https://stackoverflow.com/q/712279/5744492
class Vertex : public std::enable_shared_from_this<Vertex> {
    std::map<vertex_id, edge_ptr> _edges; ///< adjancency list

    vertex_id _id;
    vertex_value_t _val;

  public:
    Vertex() = delete;               ///< verticex are not default constructed
    Vertex(const Vertex &) = delete; ///< nor copy constructable
    Vertex(Vertex &&other) = delete; ///< nor constructable via a move

    /// \brief a vertex has an id an a value, it has no edges by default
    ///
    /// \param id vertex id
    /// \param val vertex value
    Vertex(vertex_id id, const vertex_value_t &val);

    Vertex &operator=(const Vertex &other) = delete; ///< cannot be copied
    Vertex &operator=(Vertex &&other) = delete;      ///< not ca they be moved

    /// \brief get all the edges of the vertex as pair of ids, the first element
    /// if the vertex's id
    ///
    /// \return a vector of all edges concerning this vertex
    std::vector<std::pair<vertex_id, vertex_id>> edges() const;

    /// \brief obtain the ids of all the neighbors
    ///
    /// \return vector of all the ids of the neighbors of this vertex
    std::vector<vertex_id> neighbors() const;

    vertex_id identity() const;            ///< accessor to the identity
    const vertex_value_t &value() const;   ///< accessor to value
    void value(const vertex_value_t &val); ///< mutator of the value

    /// \return true if vertex v is adjacent to this vertex
    bool adjacent(vertex_pref v) const;

    /// \brief add a directed edge from vertex v to vertex u
    ///
    /// \param v pointer to vertex as sink to the edge
    /// \param wei weight of the edge
    void add_directed_edge(vertex_pref v, const edge_weight_t &wei);

    /// \brief add an undirected edge from vertex v to vertex u. basically as
    /// adding two directed edges between this vertex and v
    ///
    /// which means adding another edge to the adjacency list of vertex v.
    /// both edges have the same weight
    ///
    /// \param v pointer to vertex as sink of the edge
    /// \param wei weight of the edge
    void add_edge(vertex_pref v, const edge_weight_t &wei);

    /// \brief overload od add_edge() where we can specify the weight to
    /// residual edge
    ///
    /// edge from this vertex to v has weight wei, where edge from vertex v to
    /// this vertex has weight re_wei
    ///
    /// \param v pointer to vertex as sink of the edge
    /// \param wei weight of the edge
    /// \param wei weight of the residual edge
    void add_edge(vertex_pref v, const edge_weight_t &wei,
                  const edge_weight_t &re_wei);

    /// \brief remove edge to vertex v
    ///
    /// \param v pointer to the vertex we want to remove edge to
    void remove_directed_edge(vertex_pref v);

    /// \brief removes both the edge and its residual, meaning it removes the
    /// edge from this vertex and from vertex v
    ///
    /// \param v pointer to the vertex we want to remove edges to and from
    void remove_edge(vertex_pref v);
};

/// \brief Graph representation as a set of vertices where each vertex holds its
/// edges. it provides handy way to manipulate the vertices and their edges
/// using ids only without the overhead of pointers
class Graph {
    std::map<vertex_id, vertex_ptr> _vertices; ///< all vertices in the graph

  public:
    Graph() = default;             ///< default graph is empty
    Graph(const Graph &) = delete; ///< graph cannot be copied

    Graph(Graph &&other) noexcept; ///< but can be moved

    /// \brief generates a graph with n_vertices and has edges picked based on
    /// their density compared to the total number of edges in a complete graph
    ///
    /// \param n_vertices number of vertices in the graph
    /// \param edge_density a value decimal between 0.0 and 1.0
    Graph(int n_vertices, double edge_density);

    /// \brief constructs a graph based on the provides vertices and edges
    ///
    /// \param vertices vector of vertex ids and their values
    /// \param edges vector of pair of vertex ids and their weights
    /// \param directed if true, all edges a directed
    Graph(const std::vector<std::pair<vertex_id, vertex_value_t>> &vertices,
          const std::vector<
              std::pair<std::pair<vertex_id, vertex_id>, edge_weight_t>> &edges,
          bool directed = true);

    Graph &operator=(const Graph &other) = delete; ///< graph cannot be copied
    Graph &operator=(Graph &&other) noexcept;      ///< but can be moved

    /// \return true if vertex u is in the graph
    bool has_vertex(vertex_id u) const;

    /// \return true if from and to vertices are adjacent
    bool adjacent(vertex_id from, vertex_id to) const;

    /// \brief handy version of adjacent()
    bool adjacent(std::pair<vertex_id, vertex_id>) const;

    /// \brief get a vector of all the vertices in the graph
    ///
    /// \return vector of all vertex id in the graph
    std::vector<vertex_id> vertices() const;

    /// \brief get a vector of all edges in the graph
    ///
    /// \return vector of all pair vertex ids representing edges in the graph
    std::vector<std::pair<vertex_id, vertex_id>> edges() const;

    /// \brief accessor to vertex value, throws if vertex is not in graph
    ///
    /// \param u vertex to get value of
    ///
    /// \return value to the vertex
    const vertex_value_t &value(vertex_id u) const;

    /// \brief mutator of vertex value, throws if vertex is not found
    ///
    /// \param u vertex to alter
    /// \param val new value of the vertex
    void value(vertex_id u, const vertex_value_t &val);

    /// \brief get all edges of a certain vertex
    ///
    /// \param u vertex to get its edges
    ///
    /// \return vector of all edges of u
    std::vector<std::pair<vertex_id, vertex_id>> edges(vertex_id u) const;

    /// \brief get all neighbors of a certain vertex
    ///
    /// \param u vertex to get its neighbors
    ///
    /// \return vector of ids of all the neighbors
    std::vector<vertex_id> neighbors(vertex_id u) const;

    /// \brief add a vertex to the graph
    ///
    /// \param u vertex to add
    /// \param val value of the vertex
    void add_vertex(vertex_id u, const vertex_value_t &val);

    /// \brief removes a vertex from the graph, all its edges are removed too
    ///
    /// \param u vertex to remove
    void remove_vertex(vertex_id u);

    /// \brief accessor of the weight of a certain edge, throws if edge is not
    /// found
    ///
    /// \param e edge as pair of vertex ids
    ///
    /// \return edge weight
    const edge_weight_t &weight(std::pair<vertex_id, vertex_id> e) const;

    /// \brief mutator of the weight of a certain edge
    ///
    /// \param e edge as pair of vertex ids
    /// \param wei new weight of the edge
    void weight(std::pair<vertex_id, vertex_id> e, const edge_weight_t &wei);

    /// \brief add a directed edge between to vertices, throws of either vertex
    /// is not found
    ///
    /// \param e a pair of vertex ids
    /// \param wei the directed edge weight
    void add_directed_edge(std::pair<vertex_id, vertex_id> e,
                           const edge_weight_t &wei);

    /// \brief same as add_directed_edge(), except if either vertex is not
    /// found, it's added to the graph with value 0
    void create_directed_edge(std::pair<vertex_id, vertex_id> e,
                              const edge_weight_t &wei);

    /// \brief add undirected edge between two vertices, throws if either is not
    /// found
    ///
    /// \param e a pair of vertex ids
    /// \param wei the weight of both edges
    void add_edge(std::pair<vertex_id, vertex_id> e, const edge_weight_t &wei);

    /// \brief same ass add_edge(), except if either vertex is not
    /// found, it's added to the graph with value 0
    void create_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei);

    /// \brief add undirected edge between two vertices, throws if either is not
    /// found. where both the edge and it's residual have different weights
    ///
    /// \param e a pair of vertex ids
    /// \param wei the edge weight
    /// \param wei the weight of the residual edge
    void add_edge(std::pair<vertex_id, vertex_id> e, const edge_weight_t &wei,
                  const edge_weight_t &re_wei);

    /// \brief same ass add_edge(), except if either vertex is not
    /// found, it's added to the graph with value 0
    void create_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei, const edge_weight_t &re_wei);

    /// \brief removes a directed edge between two vertices, throws if the edge
    /// is not found or either vertex is not found
    ///
    /// \param e a pair of vertex ids
    void remove_directed_edge(std::pair<vertex_id, vertex_id> e);

    /// \brief same as remove_directed_edge(), except it removes both edges as
    /// if there was an undirected edge
    void remove_edge(std::pair<vertex_id, vertex_id> e);

    /// \brief unit testing  of all classes functionalities
    static void unit_testing() noexcept;

  private:
    template <typename... Args>
    void vertex_check(bool in, vertex_id id, const Args &... msg) const {
        using List = int[];
        std::ostringstream stream;
        (void)List{0, ((void)(stream << msg), 0)...};

        auto itr = _vertices.find(id);
        if (in) {
            if (itr == std::end(_vertices))
                throw std::runtime_error(stream.str());
        } else {
            if (itr != std::end(_vertices))
                throw std::runtime_error(stream.str());
        }
    }
};
#endif /* GRAPH_H */
