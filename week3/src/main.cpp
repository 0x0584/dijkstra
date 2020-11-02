#include <iomanip>

#include "graph.hpp"

std::size_t n_verts = 0, n_edges = 0;

int main(int, char const *[]) {
    {
        Graph g;
        int n, m;

        std::cin >> n >> m;
        while (n--) {
            int u, v;
            std::cin >> u >> v;
            if (not g.has_vertex(u))
                g.add_vertex(u, u);
            if (not g.has_vertex(v))
                g.add_vertex(v, v);
            g.add_directed_edge(u, v, 1);
        }
        n_verts = g.vertices().size(), n_edges = g.edges().size();
        std::cout << "graph has " << n_verts << " vertices and " << n_edges
                  << " edges\n";
    }
    {
        Graph k;
        for (std::size_t i = 0; i < n_verts; ++i) {
            for (std::size_t j = 0; j < n_verts; ++j) {
                if (i == j)
                    continue;
                if (not k.has_vertex(i))
                    k.add_vertex(i, 1);
                if (not k.has_vertex(j))
                    k.add_vertex(j, 1);
                k.add_directed_edge(i, j, 1);
            }
        }
        std::cout << "complete graph has " << k.edges().size() << " edges\n";
        std::cout << std::endl;
    }

    for (double d = 0; d <= 1.05; d += 0.05) {
        Graph h{n_verts, std::min(d, 1.0)};
        std::cout << "edge density: " << std::setprecision(3) << d << " has "
                  << h.edges().size() << " edges\n";
    }
    return 0;
}
