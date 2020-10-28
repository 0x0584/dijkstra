#include "graph.hpp"

Vertex::Vertex(Vertex &&other) noexcept
    : _edges(std::exchange(other._edges, {})), id(other.id), val(other.val) {}

Vertex::Vertex(int id, int val) : id(id), val(val) {}
Vertex::~Vertex() { _edges.clear(); }

Vertex &Vertex::operator=(const Vertex &other) {
    if (this != &other)
        return *this;
    return *this;
}

Vertex &Vertex::operator=(Vertex &&other) noexcept {
    if (this == &other)
        return *this;
    return *this;
}

int Vertex::identity() const { return id; }
int Vertex::value() const { return val; }
void Vertex::value(int value) { val = value; }

std::vector<edge_t> Vertex::edges() const {
    std::vector<edge_t> vec;
    vec.reserve(_edges.size());
    std::transform(range(_edges),
                   std::back_inserter(vec),
                   [](const auto &pair) { return pair.second; });
    return vec;
}

std::vector<vertex_t> Vertex::neighbors() const {
    std::vector<vertex_t> vec;
    vec.reserve(_edges.size());
    std::transform(std::begin(_edges), std::end(_edges),
                   std::back_inserter(vec),
                   [](const auto &pair) { return pair.first; });
    return vec;
}

edge_t Vertex::edge(vertex_t v) const {
    if (not v)
        throw std::runtime_error("Vertex was null");
    if (_edges.find(v) == std::end(_edges))
        return nullptr;
    return _edges.at(v);
}

bool Vertex::adjacent(vertex_t v) const {
    if (not v)
        throw std::runtime_error("Vertex was null");
    return _edges.find(v) != std::end(_edges);
}

void Vertex::add_directed_edge(vertex_t v, int w) {
    if (not adjacent(v))
        _edges[v] = new Edge{this, v, w};
}

void Vertex::add_edge(vertex_t v, int w) { add_edge(v, w, w); }
void Vertex::add_edge(vertex_t v, int w, int re_w) {
    add_directed_edge(v, w);
    v->add_directed_edge(this, re_w);
}

void Vertex::remove_directed_edge(vertex_t v) {
    if (not v)
        throw std::runtime_error("Vertex was null");
    _edges.erase(v);
}

void Vertex::remove_edge(vertex_t v) {
    remove_directed_edge(v);
    v->remove_directed_edge(this);
}
