#include "graph.hpp"

Graph::Edge::Edge(vertex_pref from, vertex_pref to, const edge_weight_t &wei)
    : _source(from), _sink(to), _wei(wei) {
    if (not from or not to)
        throw std::runtime_error("Vertex was null");
}

Graph::vertex_ptr Graph::Edge::from() const { return _source.lock(); }
Graph::vertex_ptr Graph::Edge::to() const { return _sink.lock(); }

const Graph::edge_weight_t &Graph::Edge::weight() const { return _wei; }
void Graph::Edge::weight(const edge_weight_t &wei) { _wei = wei; }

Graph::edge_ptr Graph::Edge::make(vertex_pref from, vertex_pref to,
                                  const edge_weight_t &wei) {
    return std::make_unique<Edge>(from, to, wei);
}

Graph::edge_t Graph::Edge::make(edge_ref e) {
    const auto &_edge = e.get();
    return std::make_tuple(_edge.from()->identity(), _edge.to()->identity(),
                           _edge.weight());
}

Graph::edge_t Graph::Edge::make(edge_pref e) {
    return std::make_tuple(e->from()->identity(), e->to()->identity(),
                           e->weight());
}
