#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"

template <typename T, typename U>
struct dijkstra {
    explicit dijkstra(const Graph<T, U> &g);
};

#endif /* SHORT_PATH_H */
