#include "graph.hpp"

vertex::vertex(int id, int val) : id(id), val(val) {}
vertex::~vertex() { e.clear(); }

int vertex::identity() const { return id; }
int vertex::value() const { return val; }
void vertex::set_value(int value) { val = value; }

std::vector<edge_t> vertex::edges() const {
    std::vector<edge_t> vec;
    vec.reserve(e.size());
    for (auto [_, edge] : e)
        vec.emplace_back(edge);
    return vec;
}

std::vector<vertex_t> vertex::neighbors() const {
    std::vector<vertex_t> vec;
    vec.reserve(e.size());
    for (auto [v,_]:e)
        vec.emplace_back(v);
    return vec;
}

edge_t vertex::edge_to(vertex_t v) {
    if (not v)
        throw std::runtime_error("Vertex was null");
    if (e.find(v) == std::end(e))
        return nullptr;
    return e.at(v);
}

bool vertex::adjacent(vertex_t v) const {
    if (not v)
        throw std::runtime_error("Vertex was null");
    return e.find(v) != std::end(e);
}

void vertex::add_directed_edge(vertex_t v, int w) {
    if (not adjacent(v))
        e[v] = new edge{this, v, w};
}

void vertex::add_edge(vertex_t v, int w) { add_edge(v, w, w); }
void vertex::add_edge(vertex_t v, int w, int re_w) {
    add_directed_edge(v, w);
    v->add_directed_edge(this, re_w);
}

void vertex::remove_directed_edge(vertex_t v) {
    if (not v)
        throw std::runtime_error("Vertex was null");
    e.erase(v);
}

void vertex::remove_edge(vertex_t v) {
    remove_directed_edge(v);
    v->remove_directed_edge(this);
}
