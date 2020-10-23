#include "short_path.hpp"

std::vector<vertex> path::getpath() const { return vertices; }

int path::path_cost() const {}

void path::add_edge(const edge_t e) {
    cached = false;
    edges.emplace_back(e);
}

dijkstra::dijkstra(graph g) {}

path dijkstra::path(int s, int t) {}
