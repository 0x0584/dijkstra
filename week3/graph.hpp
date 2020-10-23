#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

typedef class vertex *vertex_t;
typedef class edge *edge_t;

class edge {
    vertex_t s, t;
    int w;

  public:
    edge(vertex_t from, vertex_t to, int weight);

    vertex_t from();
    vertex_t to();
    int weight() const;
    void set_weight(int weight);
};

class vertex {
    std::unordered_map<vertex_t, edge_t> e;
    int id, val;

  public:
    vertex(int id, int val);
    ~vertex();

    int identity() const;
    int value() const;
    void set_value(int value);
    std::vector<edge_t> edges() const;
    std::vector<vertex_t> neighbors() const;
    edge_t edge_to(vertex_t v);
    bool adjacent(vertex_t v) const;
    void add_directed_edge(vertex_t v, int w);
    void add_edge(vertex_t v, int w);
    void add_edge(vertex_t v, int w, int re_w);
    void remove_directed_edge(vertex_t v);
    void remove_edge(vertex_t v);
};

class graph {
    std::unordered_map<int, vertex_t> v;

    void vertex_check(bool in, int id, std::string_view msg);

  public:
    graph();
    ~graph();

    std::vector<class vertex> vertices() const;
    std::vector<class edge> edges() const;

    vertex get_vertex(int u);
    int vertex_value(int u);
    std::vector<edge> edges(int u);
    std::vector<vertex> neighbors(int u);
    void add_vertex(int u, int value);
    void set_vertex_value(int u, int value);
    void remove_vertex(int u);
    bool adjacent(int from, int to);

    edge get_edge(int from, int to);
    int edge_weight(int from, int to);
    void set_edge_weight(int from, int to, int w);
    void add_directed_edge(int from, int to, int w);
    void add_edge(int from, int to, int w);
    void add_edge(int from, int to, int w, int re_w);
    void remove_edge(int from, int to);
};

#endif /* GRAPH_H */
