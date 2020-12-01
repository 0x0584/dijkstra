#include "short_path.hpp"

path::path(const std::map<vertex_id, vertex_id> &parent, vertex_id sink,
           int cost)
    : _cost(cost) {
    // keep back tracing until we reach the source eventually
    for (vertex_id tmp = sink; tmp != -1; tmp = parent.at(tmp))
        verts.push_back(tmp);
    verts.shrink_to_fit();
    // sink we have started with the sink
    std::reverse(itr_range(verts));
}

int path::cost() const { return _cost; }

const std::vector<vertex_id> &path::vertices() const {
    return verts;
}

Dijkstra::Dijkstra(const Graph &graph) : g(graph) {}

path Dijkstra::find_path(vertex_id source, vertex_id sink) {
    const int inf = 1e6; // maximum distance is set to avoid overflow
    std::map<vertex_id, int> dist;
    std::map<vertex_id, vertex_id> parent;
    PQ pq;

    if (not g.has_vertex(source))
        throw std::runtime_error("vertex " + std::to_string(source) +
                                 " is not in the graph");
    if (not g.has_vertex(sink))
        throw std::runtime_error("vertex " + std::to_string(sink) +
                                 " is not in the graph");

    for (const auto &vert : g.vertices()) {
        dist[vert] = inf;  // assume all vertices are far far away
        parent[vert] = -1; // assume all vertices are unreachable
        pq.push(vert, inf);
    }

    dist[source] = 0;                 // starting at the source
    pq.change_priority(source, 0);    // which has the least priority
    while (not pq.empty()) {          // while we still have vertices to check
        auto [vert, prio] = pq.top(); // take the vertex
        pq.pop();                     // remove it from the priority queue
        if (vert == sink) // if we reach the sink no need to go further
            break; // this would happen because it would have the least priority
        for (const auto &nei : g.neighbors(vert)) { // traverse all neighbours
            if (not pq.contains(nei)) // but only if they're still undiscovered
                continue;
            // compute the alternative cost
            const int alt = dist[vert] + g.weight({vert, nei});
            if (alt < dist[nei]) {  // apply relaxation if the it's better
                dist[nei] = alt;    // update the distance
                parent[nei] = vert; // track which edge we took
                pq.change_priority(nei, alt); // change the priority accordingly
            }
        }
    }
    if (dist[sink] == inf) // if true then the sink is unreachable
        return {};
    else // we construct the path using the back trace
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
