#include "graph.hpp"

edge::edge(vertex_t from, vertex_t to, int weight) : s(from), t(to), w(weight) {
    if (not from or not to)
        throw std::runtime_error("Vertex was null");
}

vertex_t edge::from() { return s; }
vertex_t edge::to() { return t; }

int edge::weight() const { return w; }
void edge::set_weight(int weight) { w = weight; };
