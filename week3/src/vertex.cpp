#include "graph.hpp"

Graph::Vertex::Vertex(vertex_id id, const vertex_value_t &val)
    : _id(id), _val(val) {}

Graph::vertex_id Graph::Vertex::identity() const { return _id; }
const Graph::vertex_value_t &Graph::Vertex::value() const { return _val; }
void Graph::Vertex::value(const vertex_value_t &val) { _val = val; }

const Graph::edge_ptr &Graph::Vertex::edge(vertex_pref v) const {
    return _edges.at(v->_id);
}

bool Graph::Vertex::adjacent(vertex_pref v) const {
    return _edges.find(v->_id) != std::end(_edges);
}

void Graph::Vertex::add_directed_edge(vertex_pref v, const edge_weight_t &wei) {
    if (adjacent(v))
        return;
    _edges.emplace(v->_id, Edge::make(shared_from_this(), v, wei));
}

void Graph::Vertex::add_edge(vertex_pref v, const edge_weight_t &wei) {
    add_edge(v, wei, wei);
}

void Graph::Vertex::add_edge(vertex_pref v, const edge_weight_t &wei,
                             const edge_weight_t &re_wei) {
    add_directed_edge(v, wei);
    v->add_directed_edge(shared_from_this(), re_wei);
}

void Graph::Vertex::remove_directed_edge(vertex_pref v) {
    _edges.erase(v->_id);
}

void Graph::Vertex::remove_edge(vertex_pref v) {
    remove_directed_edge(v);
    v->remove_directed_edge(shared_from_this());
}

std::vector<Graph::edge_ref> Graph::Vertex::edges() const {
    const auto resolve = [](const auto &pair) {
        return std::reference_wrapper(*pair.second);
    };
    std::vector<edge_ref> vec;
    vec.reserve(_edges.size());
    std::transform(itr_range(_edges), std::back_inserter(vec), resolve);
    return vec;
}

std::vector<Graph::vertex_ref> Graph::Vertex::neighbors() const {
    const auto resolve = [](const auto &pair) {
        return std::reference_wrapper(*pair.second->to());
    };
    std::vector<vertex_ref> vec;
    vec.reserve(_edges.size());
    std::transform(itr_range(_edges), std::back_inserter(vec), resolve);
    return vec;
}

Graph::vertex_t Graph::Vertex::make(vertex_ref v) {
    return std::make_pair(v.get().identity(), v.get().value());
}

Graph::vertex_t Graph::Vertex::make(vertex_pref v) {
    return std::make_pair(v->identity(), v->value());
}
