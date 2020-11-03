#include "short_path.hpp"

path::path(const std::map<vertex_id, vertex_id> &parent,
                     vertex_id sink, int cost)
    : _cost(cost) {
    for (vertex_id tmp = sink; tmp != -1; tmp = parent.at(tmp))
        verts.push_back(tmp);
    verts.shrink_to_fit();
    std::reverse(itr_range(verts));
}

int path::cost() const { return _cost; }

const std::vector<vertex_id> &path::vertices() const {
    return verts;
}

Dijkstra::Dijkstra(const Graph &graph) : g(graph) {}

path Dijkstra::find_path(vertex_id source, vertex_id sink) {
    const int inf = 1e6;
    std::map<vertex_id, int> dist;
    std::map<vertex_id, vertex_id> parent;
    PQ pq;

    for (const auto &vert : g.vertices()) {
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
        for (const auto &nei : g.neighbors(vert)) {
            if (not pq.contains(nei))
                continue;
            const int alt = dist[vert] + g.weight({vert, nei});
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
    const auto test = [](double d) {
        Graph _g{50, d};
        Dijkstra algo{_g};

        auto &&verts = _g.vertices();
        std::pair<int, int> avg = {0, 0};

        for (unsigned j = 1; j < verts.size(); ++j)
            if (path path = algo.find_path(verts.front(), verts[j]);
                path.cost())
                avg.first += path.cost(), avg.second++;
        std::cout << "graph has " << verts.size() << " vertices and "
                  << _g.edges().size()
                  << " edges has path cost: " << (avg.first / avg.second)
                  << "\n";
    };

    test(0.2), test(0.4);
}
