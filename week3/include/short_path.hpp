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
             int cost)
            : _cost(cost) {
            for (vertex_id tmp = sink; tmp != -1; tmp = parent.at(tmp))
                verts.push_back(tmp);
            verts.shrink_to_fit();
            std::reverse(itr_range(verts));
        }

        int cost() const { return _cost; }

        const std::vector<vertex_id> &vertices() const { return verts; }

      private:
        std::vector<vertex_id> verts;
        std::size_t _cost = 0;
    };

    explicit Dijkstra(const Graph &graph) : g(graph) {}

    path find_path(vertex_id source, vertex_id sink) {
        if (not(g.has_vertex(source) and g.has_vertex(sink)))
            return {};

        // std::cout << "checking: " << source << " " << sink << "\n";

        const int max = 1e6;
        std::map<vertex_id, int> dist;
        std::map<vertex_id, vertex_id> parent;
        PQ pq;

        for (const auto &[vert, _] : g.vertices()) {
            dist[vert] = max;
            parent[vert] = -1;
            pq.push(vert, max);
        }

        dist[source] = 0;
        pq.change_priority(source, 0);
        while (not pq.empty()) {
            auto [vert, prio] = pq.top();
            // std::cout << "vertex: " << vert << " prio " << prio << "\n";
            pq.pop();
            if (vert == sink) {
                // std::cout << "found!\n";
                break;
            }
            for (const auto &[nei, _] : g.neighbors(vert)) {
                if (not pq.contains(nei))
                    continue;
                const int alt = dist[vert] + std::get<2>(g.edge(vert, nei));
                if (alt < dist[nei]) {
                    dist[nei] = alt, parent[nei] = vert;
                    pq.change_priority(nei, alt);
                }
            }
        }
        if (dist[sink] == max)
            return {};
        // std::cout << "found!\n\n";
        return {parent, sink, dist[sink]};
    }

    static void unit_testing() noexcept {
        for (double d = 0; d < 1; d += 0.1) {
            Graph _g{5, 0.8};
            Dijkstra algo{_g};
            const auto &verts = _g.vertices();
            std::cout << "graph has " << verts.size() << " vertices and "
                      << _g.edges().size() << " edges\n";
            for (const auto &[u, _] : verts) {
                for (const auto &[v, _] : verts) {
                    if (u == v)
                        continue;
                    if (path path = algo.find_path(u, v); path.cost()) {
                        std::cout << " " << u << " - " << v << ":";
                        for (const auto &e : path.vertices())
                            std::cout << " " << e;
                        std::cout << " > cost " << path.cost() << std::endl;
                    }
                }
            }
        }
    }
};

#endif /* SHORT_PATH_H */
