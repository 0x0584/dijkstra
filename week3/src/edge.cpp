#include "graph.hpp"

template <typename T, typename U>
Graph<T, U>::Edge::Edge(Edge &&other) noexcept
    : _source(std::exchange(other.s, nullptr)),
      _sink(std::exchange(other.t, nullptr)), _wei(other.w) {}

template <typename T, typename U>
Graph<T, U>::Edge::Edge(vertex_constref from, vertex_constref to,
                        edge_weight_t weight)
    : _source(from), _sink(to), _wei(weight) {
    if (not from or not to)
        throw std::runtime_error("Vertex was null");
}

template <typename T, typename U>
auto &Graph<T, U>::Edge::operator=(Edge &&other) noexcept {
    if (this == &other)
        return *this;
    return *this;
}

template <typename T, typename U> auto Graph<T, U>::Edge::from() const {
    return _source;
}

template <typename T, typename U> auto Graph<T, U>::Edge::to() const {
    return _sink;
}

template <typename T, typename U> auto Graph<T, U>::Edge::weight() const {
    return _wei;
}

template <typename T, typename U>
void Graph<T, U>::Edge::weight(edge_weight_t wei) {
    _wei = wei;
}

// template <typename T, typename U>
// bool Graph<T, U>::Edge::residual(edge_ptr other) const {
//     return other->from() == to() and from() == other->to();
// }
