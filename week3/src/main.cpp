#include "graph.hpp"

int main(int, char const *[]) {
    Graph g;

    int n, m;
    std::cin >> n >> m;
    while (n--) {
        int u, v;
        std::cin >> u >> v;
        try {
            g.add_vertex(u, 1);
        } catch (const std::runtime_error &e) {
        }
        try {
            g.add_vertex(v, 1);
        } catch (const std::runtime_error &e) {
        }
        g.add_directed_edge(u, v, 1);
    }

    for (const auto &v : g.vertices()) {
        std::cout << "(" << v.first << "):\n ";
        for (const auto &e : g.edges(v.first))
            std::cout << " (" << std::get<1>(e) << ")";
        std::cout << "\n\n";
    }
    return 0;
}
