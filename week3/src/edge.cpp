#include "graph.hpp"

Edge::Edge(vertex_pref from, vertex_pref to, const edge_weight_t &wei)
    : _source(from), _sink(to), _wei(wei) {
    if (not from or not to)
        throw std::runtime_error("Vertex was null");
}

vertex_ptr Edge::from() const { return _source.lock(); }
vertex_ptr Edge::to() const { return _sink.lock(); }

const edge_weight_t &Edge::weight() const { return _wei; }
void Edge::weight(const edge_weight_t &wei) { _wei = wei; }
