#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#define range(cont) std::begin(cont), std::end(cont)

// FIXME: implement copy constructors
// FIXME: implement assignment operators

// XXX: use smart pointers instead of raw pointers
// XXX: use templates everywhere

// TODO: reimplement public API

template <typename T = int, typename W = int> class Graph {
  public:
    using vertex_id = std::size_t;
    using vertex_value_t = T;
    using vertex_t = std::pair<vertex_id, vertex_value_t>;

    using edge_weight_t = W;
    using edge_t = std::tuple<vertex_id, vertex_id, edge_weight_t>;

  protected:
    class Vertex;
    class Edge;

    using vertex_ptr = std::shared_ptr<class Vertex>;
    using vertex_wptr = std::weak_ptr<class Vertex>;
    using vertex_constref = const vertex_ptr &;

    using edge_ptr = std::shared_ptr<class Edge>;
    using edge_constref = const edge_ptr &;

    class Vertex : std::enable_shared_from_this<Vertex> {
        std::unordered_map<vertex_wptr, edge_ptr> _edges;
        vertex_id _id;
        vertex_value_t _val;

      public:
        Vertex() = delete;
        Vertex(const Vertex &) = delete;

        Vertex(Vertex &&other) noexcept;
        Vertex(vertex_id id, vertex_value_t val);

        auto &operator=(Vertex &&other) noexcept;

        auto identity() const;
        auto value() const;
        void value(vertex_value_t val);

        auto edges() const;
        auto neighbors() const;

        auto edge(vertex_constref v) const;
        bool adjacent(vertex_constref v) const;

        void add_directed_edge(vertex_constref v, edge_weight_t wei);
        void remove_directed_edge(vertex_constref v);
        void add_edge(vertex_ptr v, edge_weight_t wei);
        void add_edge(vertex_ptr v, edge_weight_t wei, edge_weight_t re_wei);
        void remove_edge(vertex_ptr v);

        static vertex_ptr make_shared(vertex_value_t val) {
            return std::make_shared(val);
        }
    };

    class Edge : std::enable_shared_from_this<Edge> {
        vertex_wptr _source, _sink;
        edge_weight_t _wei;

      public:
        Edge() = delete;
        Edge(const Edge &) = delete;

        Edge(Edge &&other) noexcept;
        Edge(vertex_constref from, vertex_constref to, edge_weight_t wei);

        auto &operator=(Edge &&other) noexcept;

        auto from() const;
        auto to() const;
        auto weight() const;
        void weight(edge_weight_t wei);

        // FIXME: implement residual edge
        // bool residual(const edge_ptr & other) const;

        static edge_ptr make_shared(vertex_constref from, vertex_constref to,
                                    edge_weight_t wei) {
            return std::make_shared(from, to, wei);
        }
    };

    vertex_t get_vertex(vertex_constref v) const;
    edge_t get_edge(edge_constref e) const;

  public:
    Graph() = default;
    Graph(const Graph &) = delete;

    Graph(Graph &&other) noexcept;
    Graph(std::size_t n_vertices, double edge_density);
    Graph(const std::vector<vertex_t> &vertices,
          const std::vector<edge_t> &edges);

    auto &operator=(Graph &&other) noexcept;

    auto vertices() const;
    auto edges() const;

    auto vertex(vertex_id u) const;
    auto value(vertex_id u) const;
    void value(vertex_id u, vertex_value_t val) const;
    auto edges(vertex_id u) const;
    auto neighbors(vertex_id u) const;
    void add_vertex(vertex_id u, vertex_value_t va);
    void remove_vertex(vertex_id u);

    bool adjacent(vertex_id from, vertex_id to) const;

    auto edge(vertex_id from, vertex_id to) const;
    auto weight(vertex_id from, vertex_id to) const;
    void weight(vertex_id from, vertex_id to, edge_weight_t wei) const;
    void add_directed_edge(vertex_id from, vertex_id to,
                           edge_weight_t wei) const;
    void add_edge(vertex_id from, vertex_id to, edge_weight_t wei) const;
    void add_edge(vertex_id from, vertex_id to, edge_weight_t wei,
                  edge_weight_t re_wei) const;
    void remove_edge(vertex_id from, vertex_id to) const;

  private:
    std::unordered_map<vertex_id, vertex_ptr> _vertices;

    template <typename... Args>
    void vertex_check(bool in, vertex_id id, const Args &... msg) const;
};

#endif /* GRAPH_H */
