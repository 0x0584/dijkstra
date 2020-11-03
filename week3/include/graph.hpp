#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#define itr_range(cont) std::begin(cont), std::end(cont)

class Vertex;
class Edge;

using vertex_id = int;
using vertex_value_t = int;
using edge_weight_t = int;

using vertex_ptr = std::shared_ptr<Vertex>;
using vertex_pref = const vertex_ptr &;
using vertex_wptr = std::weak_ptr<Vertex>;

using edge_ptr = std::unique_ptr<Edge>;
using edge_pref = const edge_ptr &;

class Edge {
    vertex_wptr _source, _sink;
    edge_weight_t _wei;

  public:
    Edge() = delete;
    Edge(const Edge &) = delete;
    Edge(Edge &&other) = delete;

    Edge(vertex_pref from, vertex_pref to, const edge_weight_t &wei);

    Edge &operator=(const Edge &other) = delete;
    Edge &operator=(Edge &&other) = delete;

    vertex_ptr from() const;
    vertex_ptr to() const;
    const edge_weight_t &weight() const;
    void weight(const edge_weight_t &wei);
};

class Vertex : public std::enable_shared_from_this<Vertex> {
    std::map<vertex_id, edge_ptr> _edges;

    vertex_id _id;
    vertex_value_t _val;

  public:
    Vertex() = delete;
    Vertex(const Vertex &) = delete;
    Vertex(Vertex &&other) = delete;

    Vertex(vertex_id id, const vertex_value_t &val);

    Vertex &operator=(const Vertex &other) = delete;
    Vertex &operator=(Vertex &&other) = delete;

    std::vector<std::pair<vertex_id, vertex_id>> edges() const;
    std::vector<vertex_id> neighbors() const;

    vertex_id identity() const;
    const vertex_value_t &value() const;
    void value(const vertex_value_t &val);
    const edge_ptr &edge(vertex_pref v) const;
    bool adjacent(vertex_pref v) const;
    void add_directed_edge(vertex_pref v, const edge_weight_t &wei);
    void add_edge(vertex_pref v, const edge_weight_t &wei);
    void add_edge(vertex_pref v, const edge_weight_t &wei,
                  const edge_weight_t &re_wei);
    void remove_directed_edge(vertex_pref v);
    void remove_edge(vertex_pref v);
};

class Graph {
    std::map<vertex_id, vertex_ptr> _vertices;

  public:
    Graph() = default;
    Graph(const Graph &) = delete;

    Graph(Graph &&other) noexcept;
    Graph(int n_vertices, double edge_density);
    Graph(const std::vector<std::pair<vertex_id, vertex_value_t>> &vertices,
          const std::vector<
              std::pair<std::pair<vertex_id, vertex_id>, edge_weight_t>> &edges,
          bool directed = true);

    Graph &operator=(const Graph &other) = delete;
    Graph &operator=(Graph &&other) noexcept;

    bool has_vertex(vertex_id u) const;
    bool adjacent(vertex_id from, vertex_id to) const;
    bool adjacent(std::pair<vertex_id, vertex_id>) const;
    std::vector<vertex_id> vertices() const;
    std::vector<std::pair<vertex_id, vertex_id>> edges() const;

    const vertex_value_t &value(vertex_id u) const;
    void value(vertex_id u, const vertex_value_t &val);
    std::vector<std::pair<vertex_id, vertex_id>> edges(vertex_id u) const;
    std::vector<vertex_id> neighbors(vertex_id u) const;
    void add_vertex(vertex_id u, const vertex_value_t &val);
    void remove_vertex(vertex_id u);

    const edge_weight_t &weight(std::pair<vertex_id, vertex_id>) const;
    void weight(std::pair<vertex_id, vertex_id> e, const edge_weight_t &wei);
    void add_directed_edge(std::pair<vertex_id, vertex_id> e,
                           const edge_weight_t &wei) const;
    void add_edge(std::pair<vertex_id, vertex_id> e,
                  const edge_weight_t &wei) const;
    void add_edge(std::pair<vertex_id, vertex_id> e, const edge_weight_t &wei,
                  const edge_weight_t &re_wei) const;
    void remove_edge(std::pair<vertex_id, vertex_id> e) const;
    void remove_directed_edge(std::pair<vertex_id, vertex_id> e) const;

    static void unit_testing() noexcept;

  private:
    template <typename... Args>
    void vertex_check(bool in, vertex_id id, const Args &... msg) const {
        using List = int[];
        std::ostringstream stream;
        (void)List{0, ((void)(stream << msg), 0)...};

        auto itr = _vertices.find(id);
        if (in) {
            if (itr == std::end(_vertices))
                throw std::runtime_error(stream.str());
        } else {
            if (itr != std::end(_vertices))
                throw std::runtime_error(stream.str());
        }
    }
};
#endif /* GRAPH_H */
