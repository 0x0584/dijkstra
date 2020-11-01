#include "graph.hpp"

int main(int, char const *[]) {
    const std::vector<int> ids = {1, 2, 3, 4};
    Graph g;

    for (const auto &id : ids) {
        std::cout << "vertex: " << id << std::endl;
        g.add_vertex(id, id);
    }

    for (const auto &v : g.vertices())
        std::cout << "vertex: " << v.first << " " << v.second << std::endl;

    for (int i = 0; i < int(ids.size()); i += 2) {
        for (int j = 1; j < int(ids.size()); j += 2) {
            std::cout << "edge: " << ids[i] << " " << ids[j] << std::endl;
            g.add_edge(ids[i], ids[j], i + j, i - j);
        }
    }

    g.show_pointer_stats();

    std::cout << " ------------- " << std::endl;
    for (const auto &v : g.vertices()) {
        std::cout << v.first << ": " << v.second << "\n";
        for (const auto &e : g.edges(v.first)) {
            std::cout << " -> " << std::get<1>(e) << " " << std::get<2>(e)
                      << "\n";
        }
    }

    return 0;
}
