#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include "graph.hpp"
#include "pq.hpp"

/// \brief helper structure to hold a path between two vertices
///
/// providing an std::vector of vertices
/// and the paths cost (sum of weight of all edges). not intended to be used
/// alone
struct path {
    path() = default; ///< default constructor in case there was no path

    /// \brief constructor populating vertices vector and the total path cost
    ///
    /// \param parent back trace indicating which edge we took
    /// \param sink if a path was found, sink should have a predecessor
    /// \param cost the sum of all edges
    path(const std::map<vertex_id, vertex_id> &parent, vertex_id sink,
         int cost);

    int cost() const;                               ///< path cost accessor
    const std::vector<vertex_id> &vertices() const; ///< vertices accessor

  private:
    std::vector<vertex_id> verts;
    int _cost = 0;
};

/// \brief Dijkstra's shortest path algorithm
///
/// implementation uses a priority queue to prioritize which edges to take next
/// based on their weights.
struct Dijkstra {
    const Graph &g; ///< graph constant reference

    /// \brief constructor does nothing besides setting the graph
    ///
    /// \param graph in which we would operate
    explicit Dijkstra(const Graph &graph);

    /// \brief finds a path the source and the sink, throws if either is not
    /// found in the graph.
    ///
    /// \param source vertex to go from
    /// \param sink vertex to go to
    /// \return a path, if not path is found default is returned
    path find_path(vertex_id source, vertex_id sink);

    /// \brief testing all class functions
    static void unit_testing() noexcept;
};

#endif /* SHORT_PATH_H */
