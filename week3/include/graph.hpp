#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#define range(cont) std::begin(cont), std::end(cont)

// FIXME: implement copy constructors
// FIXME: implement assignment operators
// FIXME: use smart pointers instead of raw pointers

typedef class Vertex *vertex_t;
typedef class Edge *edge_t;

class Edge {
    vertex_t s, t;
    int w;

  public:
    Edge(const Edge &) = default;
    Edge(Edge &) = default;

    Edge(Edge &&other) noexcept;
    Edge(const vertex_t from, const vertex_t to, int weight);

    Edge &operator=(const Edge &other);
    Edge &operator=(Edge &&other) noexcept;

    vertex_t from() const;
    vertex_t to() const;
    int weight() const;
    void weight(int weight);
    bool residual(const edge_t other) const;
};

class Vertex {
    std::unordered_map<vertex_t, edge_t> _edges;
    int id, val;

  public:
    Vertex(Vertex &) = default;
    Vertex(const Vertex &) = default;

    Vertex(Vertex &&other) noexcept;
    Vertex(int id, int val);
    ~Vertex();

    Vertex &operator=(const Vertex &other);
    Vertex &operator=(Vertex &&other) noexcept;

    int identity() const;
    int value() const;
    void value(int value);

    std::vector<edge_t> edges() const;
    std::vector<vertex_t> neighbors() const;

    edge_t edge(const vertex_t v) const;
    bool adjacent(const vertex_t v) const;

    void add_directed_edge(vertex_t v, int w);
    void remove_directed_edge(vertex_t v);
    void add_edge(vertex_t v, int w);
    void add_edge(vertex_t v, int w, int re_w);
    void remove_edge(vertex_t v);
};

// TODO: reimplement public API
class Graph {
    std::unordered_map<int, vertex_t> _vertices;

  public:
    Graph(Graph &) = default;
    Graph(const Graph &) = default;

    Graph();
    Graph(Graph &&g) noexcept;
    explicit Graph(int n_vertices, double edge_density = 0);
    explicit Graph(std::vector<std::pair<int, int>> vertices,
                   double edge_density = 0);
    ~Graph();

    std::vector<Vertex> vertices() const;
    std::vector<Edge> edges() const;
    std::vector<Edge> edges(int u) const;
    std::vector<Vertex> neighbors(int u) const;

    Vertex vertex(int u) const;
    int value(int u) const;
    void value(int u, int value) const;
    void add_vertex(int u, int value);
    void remove_vertex(int u);

    bool adjacent(int from, int to) const;

    Edge edge(int from, int to) const;
    int weight(int from, int to) const;
    void weight(int from, int to, int w) const;
    void add_directed_edge(int from, int to, int w) const;
    void add_edge(int from, int to, int w) const;
    void add_edge(int from, int to, int w, int re_w) const;
    void remove_edge(int from, int to) const;

  private:
    void vertex_check(bool in, int id, const std::string &msg) const;
};

#endif /* GRAPH_H */
