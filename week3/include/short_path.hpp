#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"
#include "pq.hpp"

#include <numeric>

struct Dijkstra {
    using vertex_id = Graph::vertex_id;
    using vertex_t = Graph::vertex_t;
    using edge_t = Graph::edge_t;
    using edge_weight_t = Graph::edge_weight_t;

    const Graph &g;

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

    explicit Dijkstra(const Graph &graph);

    path find_path(vertex_id source, vertex_id sink);
    static void unit_testing() noexcept;
};

#endif /* SHORT_PATH_H */
