#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"

template <typename T, typename W>
struct dijkstra {
    explicit dijkstra(const Graph<T, W> &g);
};

#endif /* SHORT_PATH_H */
