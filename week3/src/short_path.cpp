#include "short_path.hpp"

Dijkstra::path::path(const std::map<vertex_id, vertex_id> &parent,
                     vertex_id sink, int cost)
    : _cost(cost) {
    for (vertex_id tmp = sink; tmp != -1; tmp = parent.at(tmp))
        verts.push_back(tmp);
    verts.shrink_to_fit();
    std::reverse(itr_range(verts));
}

int Dijkstra::path::cost() const { return _cost; }

const std::vector<Dijkstra::vertex_id> &Dijkstra::path::vertices() const {
    return verts;
}

Dijkstra::Dijkstra(const Graph &graph) : g(graph) {}

Dijkstra::path Dijkstra::find_path(vertex_id source, vertex_id sink) {
    const int inf = 1e6;
    std::map<vertex_id, int> dist;
    std::map<vertex_id, vertex_id> parent;
    PQ pq;

    for (const auto &[vert, _] : g.vertices()) {
        dist[vert] = inf;
        parent[vert] = -1;
        pq.push(vert, inf);
    }

    dist[source] = 0;
    pq.change_priority(source, 0);
    while (not pq.empty()) {
        auto [vert, prio] = pq.top();
        pq.pop();
        if (vert == sink)
            break;
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
    if (dist[sink] == inf)
        return {};
    return {parent, sink, dist[sink]};
}

void Dijkstra::unit_testing() noexcept {
    for (double d = 0; d < 1; d += 0.1) {
        Graph _g{50, 0.8};
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
