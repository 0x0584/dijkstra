#include "graph.hpp"

int main(int, char const *[]) {
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

    std::cout << "number of vertices: " << g.vertices().size() << std::endl;
    std::cout << "number of edges: " << g.edges().size() << std::endl;


    return 0;
}
