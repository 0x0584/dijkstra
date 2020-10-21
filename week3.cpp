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
    edge(vertex_t from, vertex_t to, int weight) : s(from), t(to), w(weight) {
        if (not from or not to)
            throw std::runtime_error("Vertex was null");
    }

    vertex_t from() { return s; }
    vertex_t to() { return t; }

    int weight() const { return w; }
    void set_weight(int weight) { w = weight; }
};

class vertex {
    std::unordered_map<vertex_t, edge_t> e;
    int id, val;

  public:
    vertex(int id, int val) : id(id), val(val) {}
    ~vertex() { e.clear(); }

    int identity() const { return id; }
    int value() const { return val; }
    void set_value(int value) { val = value; }

    std::vector<edge_t> edges() const {
        std::vector<edge_t> vec;
        for (auto [_, edge] : e)
            vec.emplace_back(edge);
        vec.shrink_to_fit();
        return vec;
    }

    bool adjacent(vertex_t v) const {
        if (not v)
            throw std::runtime_error("Vertex was null");
        return e.find(v) != std::end(e);
    }

    void add_directed_edge(vertex_t v, int w) {
        if (not adjacent(v))
            e[v] = new edge{this, v, w};
    }

    void add_edge(vertex_t v, int w) { add_edge(v, w, w); }
    void add_edge(vertex_t v, int w, int re_w) {
        add_directed_edge(v, w);
        v->add_directed_edge(this, re_w);
    }

    void remove_directed_edge(vertex_t v) {
        if (not v)
            throw std::runtime_error("Vertex was null");
        e.erase(v);
    }

    void remove_edge(vertex_t v) {
        remove_directed_edge(v);
        v->remove_directed_edge(this);
    }
};

class graph {
    std::unordered_map<int, vertex_t> v;

    void vertex_check(bool in, int id, std::string_view msg) {
        auto itr = v.find(id);
        if (in) {
            if (itr == std::end(v))
                throw std::runtime_error(msg.data());
        } else {
            if (itr != std::end(v))
                throw std::runtime_error(msg.data());
        }
    }

  public:
    graph() {}
    ~graph() { v.clear(); }

    std::vector<class vertex> vertices() const {
        std::vector<vertex> vertices;
        std::transform(std::begin(v), std::end(v), std::back_inserter(vertices),
                       [](auto itr) { return *itr.second; });
        vertices.shrink_to_fit();
        return vertices;
    }

    std::vector<class edge> edges() const {
        std::vector<edge> edges;
        for (auto [_, vi] : v)
            std::transform(std::begin(vi->edges()), std::end(vi->edges()),
                           std::back_inserter(edges), [](auto edges) { return *edges; });
        edges.shrink_to_fit();
        return edges;
    }

    void add_vertex(int u, int value) {
        vertex_check(false, u, "Vertex already exists");
        v[u] = new vertex{u, value};
    }

    void set_vertex_value(int u, int value) {
        vertex_check(true, u, "Vertex not found");
        v[u]->set_value(value);
    }

    int vertex_value(int u) {
        vertex_check(true, u, "Vertex not found");
        return v[u]->value();
    }

    void remove_vertex(int u) { v.erase(u); }

    bool adjacent(int from, int to) {
        vertex_check(true, from, "Vertex not found");
        vertex_check(true, to, "Vertex not found");
        return v[from]->adjacent(v[to]);
    }

    void add_directed_edge(int from, int to, int w) {
        vertex_check(true, from, "Vertex not found");
        vertex_check(true, to, "Vertex not found");
        v[from]->add_edge(v[to], w);
    }

    void add_edge(int from, int to, int w) { add_edge(from, to, w, w); }

    void add_edge(int from, int to, int w, int re_w) {
        add_directed_edge(from, to, w);
        add_directed_edge(to, from, re_w);
    }

    void remove_edge(int from, int to) {
        vertex_check(true, from, "Vertex not found");
        vertex_check(true, to, "Vertex not found");
        v[from]->remove_edge(v[to]);
    }

    int edge_weight(int from, int to) {
        vertex_check(true, from, "Vertex not found");
        vertex_check(true, to, "Vertex not found");
    }
};

int main(int, char *[]) {
    ;
    return 0;
}
