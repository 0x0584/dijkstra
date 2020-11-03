#include "graph.hpp"

Vertex::Vertex(vertex_id id, const vertex_value_t &val) : _id(id), _val(val) {}

std::vector<std::pair<vertex_id, vertex_id>> Vertex::edges() const {
    std::vector<std::pair<vertex_id, vertex_id>> vec;
    vec.reserve(_edges.size());
    for (const auto &[nei, _] : _edges)
        vec.emplace_back(_id, nei);
    return vec;
}

std::vector<vertex_id> Vertex::neighbors() const {
    std::vector<vertex_id> vec;
    vec.reserve(_edges.size());
    for (const auto &[nei, _] : _edges)
        vec.emplace_back(nei);
    return vec;
}

vertex_id Vertex::identity() const { return _id; }
const vertex_value_t &Vertex::value() const { return _val; }
void Vertex::value(const vertex_value_t &val) { _val = val; }

const edge_ptr &Vertex::edge(vertex_pref v) const {
    return _edges.at(v->_id);
}

bool Vertex::adjacent(vertex_pref v) const {
    return _edges.find(v->_id) != std::end(_edges);
}

void Vertex::add_directed_edge(vertex_pref v, const edge_weight_t &wei) {
    if (adjacent(v))
        return;
    _edges.emplace(v->_id, std::make_unique<Edge>(shared_from_this(), v, wei));
}

void Vertex::add_edge(vertex_pref v, const edge_weight_t &wei) {
    add_edge(v, wei, wei);
}

void Vertex::add_edge(vertex_pref v, const edge_weight_t &wei,
                             const edge_weight_t &re_wei) {
    add_directed_edge(v, wei);
    v->add_directed_edge(shared_from_this(), re_wei);
}

void Vertex::remove_directed_edge(vertex_pref v) {
    _edges.erase(v->_id);
}

void Vertex::remove_edge(vertex_pref v) {
    remove_directed_edge(v);
    v->remove_directed_edge(shared_from_this());
}
