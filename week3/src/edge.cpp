#include "graph.hpp"

Edge::Edge(Edge &&other) noexcept
    : s(std::exchange(other.s, nullptr)), t(std::exchange(other.t, nullptr)),
      w(other.w) {}

Edge::Edge(vertex_t from, vertex_t to, int weight) : s(from), t(to), w(weight) {
    if (not from or not to)
        throw std::runtime_error("Vertex was null");
}

Edge &Edge::operator=(const Edge &other) {
    if (this == &other)
        return *this;
    return *this;
}

Edge &Edge::operator=(Edge &&other) noexcept {
    if (this == &other)
        return *this;
    return *this;
}

vertex_t Edge::from() const { return s; }
vertex_t Edge::to() const { return t; }

int Edge::weight() const { return w; }
void Edge::weight(int weight) { w = weight; }
