#include "graph.hpp"

int main(int, char const *[]) {
    Graph g;

    for (const auto &v : g.vertices()) {
        std::cout << "(" << v.first << ") " << v.second << "\n";
        for (const auto &e : g.edges(v.first))
            std::cout << "(" << std::get<0>(e) << ")-(" << std::get<1>(e)
                      << ") " << std::get<2>(e) << "\n";
    }
    return 0;
}
