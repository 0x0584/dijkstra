#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"

using namespace graph;

template <typename T, typename U>
struct dijkstra {
    explicit dijkstra(const Graph<T, U> &g);
};

#endif /* SHORT_PATH_H */
