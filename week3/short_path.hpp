#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"

struct path {
    std::vector<vertex> getpath() const;
    int path_cost() const;
    void add_edge(const edge_t e);

  private:
    std::vector<edge_t> edges;
    std::vector<vertex> vertices;
    mutable bool cached = false;
    mutable unsigned last = 0;
    mutable int cost = 0;
};

struct dijkstra {
    explicit dijkstra(graph g);

    path path(int s, int t);
};

#endif /* SHORT_PATH_H */
