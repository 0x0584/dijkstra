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

#define itr_range(cont) std::begin(cont), std::end(cont)

// XXX: use smart pointers instead of raw pointers
// XXX: use templates everywhere

// XXX: reimplement public API

// template <typename T = int, typename W = T>
using T = int;
using W = T;

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

    using vertex_ref = std::reference_wrapper<Vertex>;
    using vertex_ptr = std::shared_ptr<Vertex>;
    using vertex_pref = const vertex_ptr &;
    using vertex_wptr = std::weak_ptr<Vertex>;

    using edge_ref = std::reference_wrapper<Edge>;
    using edge_ptr = std::unique_ptr<Edge>;
    using edge_pref = const edge_ptr &;

    class Edge {
        vertex_wptr _source, _sink;
        edge_weight_t _wei;

      public:
        Edge() = delete;
        Edge(const Edge &) = delete;
        Edge(Edge &&other) = delete;

        Edge(vertex_pref from, vertex_pref to, const edge_weight_t &wei)
            : _source(from), _sink(to), _wei(wei) {
            if (not from or not to)
                throw std::runtime_error("Vertex was null");
        }

        Edge &operator=(const Edge &other) = delete;
        Edge &operator=(Edge &&other) = delete;

        vertex_ptr from() const { return _source.lock(); }
        vertex_ptr to() const { return _sink.lock(); }

        const edge_weight_t &weight() const { return _wei; }
        void weight(const edge_weight_t &wei) { _wei = wei; }

        bool residual(const edge_ptr &other) const {
            return from() == other->to() and to() == other->from();
        }
    };

    class Vertex : public std::enable_shared_from_this<Vertex> {
        using edge_map = std::unordered_map<vertex_id, edge_ptr>;

        edge_map _edges;
        vertex_id _id;
        vertex_value_t _val;

      public:
        Vertex() = delete;
        Vertex(const Vertex &) = delete;
        Vertex(Vertex &&other) = delete;

        Vertex(vertex_id id, const vertex_value_t &val) : _id(id), _val(val) {}

        Vertex &operator=(const Vertex &other) = delete;
        Vertex &operator=(Vertex &&other) = delete;

        vertex_id identity() const { return _id; }
        const vertex_value_t &value() const { return _val; }
        void value(const vertex_value_t &val) { _val = val; }

        const edge_ptr &edge(vertex_pref v) const { return _edges.at(v->_id); }

        bool adjacent(vertex_pref v) const {
            return _edges.find(v->_id) != std::end(_edges);
        }

        void add_directed_edge(vertex_pref v, const edge_weight_t &wei) {
            if (adjacent(v))
                return;
            vertex_wptr ptr = weak_from_this();
            _edges.emplace(v->_id, std::make_unique<Edge>(ptr.lock(), v, wei));
        }

        void add_edge(vertex_pref v, const edge_weight_t &wei) {
            add_edge(v, wei, wei);
        }

        void add_edge(vertex_pref v, const edge_weight_t &wei,
                      const edge_weight_t &re_wei) {
            add_directed_edge(v, wei);
            v->add_directed_edge(this->shared_from_this(), re_wei);
        }

        void remove_directed_edge(vertex_pref v) { _edges.erase(v->_id); }

        void remove_edge(vertex_pref v) {
            remove_directed_edge(v);
            v->remove_directed_edge(this->shared_from_this());
        }

        std::vector<edge_ref> edges() const {
            const auto resolve = [](const auto &pair) {
                return std::reference_wrapper(*pair.second);
            };
            std::vector<edge_ref> vec;
            vec.reserve(_edges.size());
            std::transform(itr_range(_edges), std::back_inserter(vec), resolve);
            return vec;
        }

        std::vector<vertex_ref> neighbors() const {
            const auto resolve = [](const auto &pair) {
                return std::reference_wrapper(*pair.second->to());
            };
            std::vector<vertex_ref> vec;
            vec.reserve(_edges.size());
            std::transform(itr_range(_edges), std::back_inserter(vec), resolve);
            return vec;
        }
    };

    using vertex_map = std::unordered_map<vertex_id, vertex_ptr>;

    std::pair<vertex_id, vertex_id> generate_edge(std::size_t limit) {
        auto generate_vertex = [&limit, this](vertex_id old = -1) {
            static std::random_device gen;
            static std::uniform_int_distribution<std::size_t> u(0, limit);
            vertex_id vertex;
            do {
                vertex = u(gen);
            } while (vertex != old and
                     _vertices.find(vertex) != std::end(_vertices));
            return vertex;
        };
        vertex_id u = generate_vertex(), v = generate_vertex(u);
        return std::make_pair(u, v);
    }

    vertex_t make_vertex(vertex_ref v) const {
        return std::make_pair(v.get().identity(), v.get().value());
    }

    vertex_t make_vertex(vertex_pref v) const {
        return std::make_pair(v->identity(), v->value());
    }

    edge_t make_edge(edge_ref e) const {
        const auto &_edge = e.get();
        return std::make_tuple(_edge.from()->identity(), _edge.to()->identity(),
                               _edge.weight());
    }

    edge_t make_edge(edge_pref e) const {
        return std::make_tuple(e->from()->identity(), e->to()->identity(),
                               e->weight());
    }

  public:
    Graph() = default;
    Graph(const Graph &) = delete;

    Graph(Graph &&other) noexcept
        : _vertices(std::exchange(other._vertices, {})) {}

    Graph(std::size_t n_vertices, double edge_density) {
        if (edge_density > 1)
            throw std::runtime_error("edge_density > 1");
        const std::size_t n_edges = (n_vertices * (n_vertices - 1)) / 2;
        for (std::size_t i = 0; i < n_vertices; ++i)
            add_vertex(i, 1);
        for (std::size_t i = 0; i < n_edges; ++i) {
            std::pair<vertex_id, vertex_id> _edge = generate_edge(n_vertices);
            add_edge(_edge.first, _edge.second, 1);
        }
    }

#define make_edge(e) std::get<0>(e), std::get<1>(e), std::get<2>(e)
    Graph(const std::vector<vertex_t> &vertices,
          const std::vector<edge_t> &edges, bool directed = true) {
        for (const auto &v : vertices)
            add_vertex(v.first, v.second);
        for (const auto &e : edges) {
            if (directed)
                add_directed_edge(make_edge(e));
            else
                add_edge(make_edge(e));
        }
    }
#undef make_edge

    Graph &operator=(const Graph &other) = delete;
    Graph &operator=(Graph &&other) noexcept {
        if (this == &other)
            return *this;
        _vertices.clear();
        _vertices = std::exchange(other._vertices, {});
        return *this;
    }

    std::vector<vertex_t> vertices() const {
        std::vector<vertex_t> nodes;
        const auto resolve = [this](const auto &pair) {
            return this->make_vertex(pair.second);
        };
        nodes.reserve(_vertices.size());
        std::transform(itr_range(_vertices), std::back_inserter(nodes),
                       resolve);
        return nodes;
    }

    std::vector<edge_t> edges() const {
        std::vector<edge_t> links;
        const auto resolve = [&links, this](const auto &pair) {
            const auto &v_edges = pair.second->edges();
            return std::transform(itr_range(v_edges), std::back_inserter(links),
                                  [this](edge_ref e) { return make_edge(e); });
        };
        std::for_each(itr_range(_vertices), resolve);
        links.shrink_to_fit();
        return links;
    }

    vertex_t vertex(vertex_id u) const {
        vertex_check(true, u, "vertex ", u, " is not found");
        return make_vertex(_vertices.at(u));
    }

    const vertex_value_t &value(vertex_id u) const {
        vertex_check(true, u, "vertex ", u, " is not found");
        return _vertices.at(u)->value();
    }

    void value(vertex_id u, const vertex_value_t &val) {
        vertex_check(true, u, "vertex ", u, " is not found");
        _vertices.at(u)->value(val);
    }

    std::vector<edge_t> edges(vertex_id u) const {
        std::vector<edge_t> links;
        auto v_edges = _vertices.at(u)->edges();
        links.reserve(v_edges.size());
        std::transform(itr_range(v_edges), std::back_inserter(links),
                       [this](edge_ref edge) { return make_edge(edge); });
        return links;
    }

    std::vector<vertex_t> neighbors(vertex_id u) const {
        vertex_check(true, u, "vertex ", u, " is not found");
        std::vector<vertex_t> nodes;
        auto neis = _vertices.at(u)->neighbors();
        nodes.reserve(neis.size());
        std::transform(
            itr_range(neis), std::back_inserter(nodes),
            [this](vertex_ref vertex) { return make_vertex(vertex); });
        return nodes;
    }

    void add_vertex(vertex_id u, const vertex_value_t &val) {
        vertex_check(false, u, "vertex ", u, " already exists");
        _vertices.emplace(u, std::make_shared<Vertex>(u, val));
    }

    void remove_vertex(vertex_id u) { _vertices.erase(u); }

    bool adjacent(vertex_id from, vertex_id to) const {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        return _vertices.at(from)->adjacent(_vertices.at(to));
    }

    edge_t edge(vertex_id from, vertex_id to) const {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        if (not adjacent(from, to))
            throw std::runtime_error("no edge between " + std::to_string(from) +
                                     " and " + std::to_string(to));
        return make_edge(_vertices.at(from)->edge(_vertices.at(to)));
    }

    const edge_weight_t &weight(vertex_id from, vertex_id to) const {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        return _vertices.at(from)->edge(_vertices.at(to))->weight();
    }

    void weight(vertex_id from, vertex_id to, const edge_weight_t &wei) {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
    }

    void add_directed_edge(vertex_id from, vertex_id to,
                           const edge_weight_t &wei) const {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        _vertices.at(from)->add_directed_edge(_vertices.at(to), wei);
    }

    void add_edge(vertex_id from, vertex_id to,
                  const edge_weight_t &wei) const {
        add_edge(from, to, wei, wei);
    }

    void add_edge(vertex_id from, vertex_id to, const edge_weight_t &wei,
                  const edge_weight_t &re_wei) const {
        add_directed_edge(from, to, wei);
        add_directed_edge(to, from, re_wei);
    }

    void remove_edge(vertex_id from, vertex_id to) const {
        vertex_check(true, from, "vertex ", from, " is not found");
        vertex_check(true, to, "vertex ", to, " is not found");
        _vertices.at(from)->remove_edge(_vertices.at(to));
    }

    void remove_directed_edge(vertex_id from, vertex_id to) const;

  private:
    vertex_map _vertices;

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
