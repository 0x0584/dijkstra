#include "graph.hpp"

template <typename T, typename W>
Graph<T, W>::Vertex::Vertex(Graph<T, W>::Vertex &&other) noexcept
    : _edges(std::exchange(other._edges, {})), _id(other.id), _val(other.val) {}

template <typename T, typename W>
auto &Graph<T, W>::Vertex::operator=(Vertex &&other) noexcept {
    if (this == &other)
        return *this;
    return *this;
}

template <typename T, typename W> auto Graph<T, W>::Vertex::identity() const {
    return _id;
}

template <typename T, typename W> auto Graph<T, W>::Vertex::value() const {
    return _val;
}

template <typename T, typename W>
void Graph<T, W>::Vertex::value(vertex_value_t val) {
    _val = val;
}

template <typename T, typename W> auto Graph<T, W>::Vertex::edges() const {
    std::vector<edge_ptr> vec;
    vec.reserve(_edges.size());
    std::transform(range(_edges), std::back_inserter(vec),
                   [](const auto &pair) { return pair.second; });
    return vec;
}

template <typename T, typename W> auto Graph<T, W>::Vertex::neighbors() const {
    std::vector<vertex_ptr> vec;
    vec.reserve(_edges.size());
    std::transform(std::begin(_edges), std::end(_edges),
                   std::back_inserter(vec),
                   [](const auto &pair) { return pair.first; });
    return vec;
}

template <typename T, typename W>
auto Graph<T, W>::Vertex::edge(vertex_constref v) const {
    if (not v)
        throw std::runtime_error("Vertex was null");
    if (_edges.find(v) == std::end(_edges))
        return nullptr;
    return _edges.at(v);
}

template <typename T, typename W>
bool Graph<T, W>::Vertex::adjacent(vertex_constref v) const {
    if (not v)
        throw std::runtime_error("Vertex was null");
    return _edges.find(v) != std::end(_edges);
}

template <typename T, typename W>
void Graph<T, W>::Vertex::add_directed_edge(vertex_constref v,
                                            edge_weight_t wei) {
    if (not adjacent(v))
        _edges.emplace(v, Edge::make_shared(this->shared_from_this(), v, wei));
}

template <typename T, typename W>
void Graph<T, W>::Vertex::add_edge(vertex_ptr v, edge_weight_t wei) {
    add_edge(v, wei, wei);
}

template <typename T, typename W>
void Graph<T, W>::Vertex::add_edge(vertex_ptr v, edge_weight_t wei,
                                   edge_weight_t re_wei) {
    add_directed_edge(v, wei);
    v->add_directed_edge(this->shared_from_this(), re_wei);
}

template <typename T, typename W>
void Graph<T, W>::Vertex::remove_directed_edge(vertex_constref v) {
    if (not v)
        throw std::runtime_error("Vertex was null");
    _edges.erase(v);
}

template <typename T, typename W>
void Graph<T, W>::Vertex::remove_edge(vertex_ptr v) {
    remove_directed_edge(v);
    v->remove_directed_edge(this->shared_from_this());
}
