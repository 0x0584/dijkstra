#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"
#include "pq.hpp"

struct path {
    path() = default;
    path(const std::map<vertex_id, vertex_id> &parent, vertex_id sink,
         int cost);
    int cost() const;
    const std::vector<vertex_id> &vertices() const;

  private:
    std::vector<vertex_id> verts;
    int _cost = 0;
};

struct Dijkstra {
    const Graph &g;

    explicit Dijkstra(const Graph &graph);

    path find_path(vertex_id source, vertex_id sink);
    static void unit_testing() noexcept;
};

#endif /* SHORT_PATH_H */
