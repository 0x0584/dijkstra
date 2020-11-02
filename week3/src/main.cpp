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
        std::size_t k = (n_verts * (n_verts - 1));
        std::cout << "complete graph has " << k << " edges\n";
        std::cout << std::endl;
    }

    for (double d = 0; d <= 1.05; d += 0.05) {
        Graph h{n_verts, std::min(d, 1.0)};
        std::cout << "edge density: " << d << " has " << h.edges().size()
                  << " edges\n";
    }
    return 0;
}
