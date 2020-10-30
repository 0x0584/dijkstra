#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
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

namespace graph {
template <typename T = int, typename W = T>
class Graph {
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

    using edge_ptr = std::shared_ptr<class Edge>;
    using edge_wptr = std::weak_ptr<class Edge>;

    class Vertex : std::enable_shared_from_this<Vertex> {
        std::unordered_map<vertex_ptr, edge_ptr> _edges;
        vertex_id _id;
        vertex_value_t _val;
        const vertex_ptr &self;

      public:
        Vertex() = delete;
        Vertex(const Vertex &) = delete;
        Vertex(Vertex &&other) = delete;

        Vertex(vertex_id id, const vertex_value_t &val)
            : _id(id), _val(val), self(this->sahred_from_this()) {}

        Vertex &operator=(const Vertex &other) = delete;
        Vertex &operator=(Vertex &&other) = delete;

        vertex_id identity() const { return _id; }
        const vertex_value_t &value() const { return _val; }
        void value(const vertex_value_t &val) { _val = val; }

        edge_wptr edge(vertex_wptr v) const {
            return adjacent(v) ? _edges.at(v) : edge_wptr{};
        }

        bool adjacent(vertex_wptr v) const {
            if (vertex_ptr ptr = v.lock(); ptr.expired())
                throw std::runtime_error("Vertex was null");
            else
                return _edges.find(ptr) != std::end(_edges);
        }

        void add_directed_edge(vertex_wptr v, const edge_weight_t &wei) {
            if (adjacent(v))
                _edges.emplace(v, Edge::make_shared(self, v, wei));
        }

        void add_edge(vertex_ptr v, const edge_weight_t &wei) {
            add_edge(v, wei, wei);
        }

        void add_edge(vertex_ptr v, const edge_weight_t &wei,
                      const edge_weight_t &re_wei) {
            add_directed_edge(v, wei);
            v->add_directed_edge(self, re_wei);
        }

        void remove_directed_edge(vertex_wptr v) {
            if (vertex_ptr ptr = v.lock(); ptr.expired())
                throw std::runtime_error("Vertex was null");
            _edges.erase(v);
        }

        void remove_edge(vertex_ptr v) {
            remove_directed_edge(v);
            v->remove_directed_edge(self);
        }

        std::vector<edge_wptr> edges() const {
            std::vector<edge_wptr> vec;
            vec.reserve(_edges.size());
            std::transform(range(_edges), std::back_inserter(vec),
                           [](const auto &pair) { return pair.second; });
            return vec;
        }

        std::vector<vertex_wptr> neighbors() const {
            std::vector<vertex_wptr> vec;
            vec.reserve(_edges.size());
            std::transform(std::begin(_edges), std::end(_edges),
                           std::back_inserter(vec),
                           [](const auto &pair) { return pair.first; });
            return vec;
        }

        static vertex_ptr make_shared(const vertex_value_t &val) {
            return std::make_shared(val);
        }
    };

    class Edge {
        vertex_wptr _source, _sink;
        edge_weight_t _wei;

      public:
        Edge() = delete;
        Edge(const Edge &) = delete;
        Edge(Edge &&other) = delete;

        Edge(vertex_wptr from, vertex_wptr to, const edge_weight_t &wei)
            : _source(from), _sink(to), _wei(wei) {
            if (not from or not to)
                throw std::runtime_error("Vertex was null");
        }

        Edge &operator=(const Edge &other) = delete;
        Edge &operator=(Edge &&other) = delete;

        vertex_ptr from() const { return _source; }
        vertex_ptr to() const { return _sink; }
        const edge_weight_t &weight() const { return _sink; }
        void weight(const edge_weight_t &wei) { _wei = wei; }

        // FIXME: implement residual edge
        bool residual(const edge_wptr &other) const {}

        static edge_ptr make_shared(vertex_wptr from, vertex_wptr to,
                                    const edge_weight_t &wei) {
            return std::make_shared(from, to, wei);
        }
    };

    vertex_t get_vertex(vertex_wptr v) const;
    edge_t get_edge(edge_wptr e) const;
    std::pair<vertex_id, vertex_id> generate_edge(std::size_t limit);

  public:
    Graph() = default;
    Graph(const Graph &) = delete;

    Graph(Graph &&other) noexcept;
    Graph(std::size_t n_vertices, double edge_density);
    Graph(const std::vector<vertex_t> &vertices,
          const std::vector<edge_t> &edges, bool directed = true);

    Graph &operator=(const Graph &other) = delete;
    Graph &operator=(Graph &&other) noexcept;

    std::vector<vertex_t> vertices() const;
    std::vector<edge_t> edges() const;

    vertex_t vertex(vertex_id u) const;
    const vertex_value_t &value(vertex_id u) const;
    void value(vertex_id u, const vertex_value_t &val) const;
    std::vector<edge_t> edges(vertex_id u) const;
    std::vector<vertex_t> neighbors(vertex_id u) const;
    void add_vertex(vertex_id u, const vertex_value_t &val);
    void remove_vertex(vertex_id u);

    bool adjacent(vertex_id from, vertex_id to) const;

    edge_t edge(vertex_id from, vertex_id to) const;
    const edge_weight_t &weight(vertex_id from, vertex_id to) const;
    void weight(vertex_id from, vertex_id to, const edge_weight_t &wei) const;

    void add_directed_edge(vertex_id from, vertex_id to,
                           const edge_weight_t &wei) const;
    void add_edge(vertex_id from, vertex_id to, const edge_weight_t &wei) const;
    void add_edge(vertex_id from, vertex_id to, const edge_weight_t &wei,
                  const edge_weight_t &re_wei) const;

    void remove_edge(vertex_id from, vertex_id to) const;
    void remove_directed_edge(vertex_id from, vertex_id to) const;

  private:
    std::unordered_map<vertex_id, vertex_ptr> _vertices;

    template <typename... Args>
    void vertex_check(bool in, vertex_id id, const Args &... msg) const;
};

// FIXME: implement operator<<

namespace impl {
template <typename T, typename W>
using Vertex = typename Graph<T, W>::Vertex;

template <typename T, typename W>
using Edge = typename Graph<T, W>::Edge;

template <typename T, typename W>
using vertex_id = typename Graph<T, W>::vertex_id;

template <typename T, typename W = void>
using vertex_value_t = typename Graph<T, W>::vertex_value_t;

template <typename T, typename W>
using vertex_t = typename Graph<T, W>::vertex_t;

template <typename T, typename W>
using edge_weight_t = typename Graph<T, W>::edge_weight_t;

template <typename T, typename W>
using edge_t = typename Graph<T, W>::edge_t;

template <typename T, typename W>
using vertex_ptr = typename Graph<T, W>::vertex_ptr;

template <typename T, typename W>
using vertex_wptr = typename Graph<T, W>::vertex_wptr;

template <typename T, typename W>
using vertex_constref = const vertex_ptr<T, W> &;

template <typename T, typename W>
using edge_ptr = typename Graph<T, W>::edge_ptr;

template <typename T, typename W>
using edge_wptr = typename Graph<T, W>::edge_wptr;

template <typename T, typename W>
using edge_constref = const edge_ptr<T, W> &;
} // namespace impl
} // namespace graph
#endif /* GRAPH_H */
