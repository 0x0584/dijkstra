#include "graph.hpp"
#include <iomanip>

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

    auto vec = g.vertices();
    std::cout << "number of vertices: " << vec.size() << "\n";
    std::cout << "number of edges: " << g.edges().size() << "\n\n";

    std::sort(itr_range(vec), [&g](const auto &e1, const auto &e2) {
        return g.edges(e1.first).size() > g.edges(e2.first).size();
    });

    for (const auto &v : vec)
        std::cout << "(" << v.first << ") has " << g.edges(v.first).size()
                  << " edges\n";
    return 0;
}
