#include "graph.hpp"

using namespace graph;
using namespace graph::impl;

template <typename T, typename W>
Graph<T, W>::Graph(Graph &&other) noexcept
    : _vertices(std::exchange(other._vertices, {})) {}

template <typename T, typename W>
Graph<T, W>::Graph(std::size_t n_vertices, double edge_density) {
    if (edge_density > 1)
        throw std::runtime_error("edge_density > 1");

    const std::size_t n_edges = (n_vertices * (n_vertices - 1)) / 2;

    for (std::size_t i = 0; i < n_vertices; ++i)
        add_vertex(i, 1);

    for (std::size_t i = 0; i < n_edges; ++i) {
        std::pair<vertex_id, vertex_id> edge = generate_edge(n_vertices);
        add_edge(edge.first, edge.second, 1);
    }
}

template <typename T, typename W>
Graph<T, W>::Graph(const std::vector<vertex_t> &vertices,
                   const std::vector<edge_t> &edges, bool directed) {
    for (const auto &v : vertices)
        add_vertex(v.first, v.second);
    const auto &which_add_edge = directed ? &add_directed_edge : &add_edge;
    for (const auto &e : edges)
        which_add_edge(std::get<0>(e), std::get<1>(e), std::get<2>(e));
}

template <typename T, typename W>
Graph<T, W> &Graph<T, W>::operator=(Graph &&other) noexcept {
    if (this == &other)
        return *this;
    _vertices.clear();
    _vertices = std::exchange(other._vertices, {});
    return *this;
}

template <typename T, typename W>
std::vector<vertex_t<T, W>> Graph<T, W>::vertices() const {
    std::vector<vertex_t> nodes;
    nodes.reverse(_vertices.size());
    std::transform(range(_vertices), std::back_inserter(nodes),
                   [this](auto pair) { return get_vertex(pair->second); });
    return nodes;
}

template <typename T, typename W>
std::vector<edge_t<T, W>> Graph<T, W>::edges() const {
    std::vector<edge_t> links;
    std::for_each(range(_vertices), [&links, this](const auto &pair) {
        auto v_edges = pair.second->edges();
        return std::transform(
            range(v_edges), std::back_inserter(links),
            [this](const auto &edge) { return get_edge(edge); });
    });
    links.shrink_to_fit();
    return links;
}

template <typename T, typename W>
vertex_t<T, W> Graph<T, W>::vertex(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return get_vertex(_vertices.at(u));
}

template <typename T, typename W>
void Graph<T, W>::value(vertex_id u, const vertex_value_t &val) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    _vertices.at(u)->value(val);
}

template <typename T, typename W>
const vertex_value_t<T, W> &Graph<T, W>::value(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->value();
}

template <typename T, typename W>
std::vector<edge_t<T, W>> Graph<T, W>::edges(vertex_id u) const {
    auto v_edges = _vertices.at(u)->edges();
    std::vector<edge_t> links;
    links.reserve(v_edges.size());
    std::transform(range(v_edges), std::back_inserter(links),
                   [this](const auto &edge) { return get_edge(edge); });
    return links;
}

template <typename T, typename W>
std::vector<vertex_t<T, W>> Graph<T, W>::neighbors(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    auto neis = _vertices.at(u)->neighbors();
    std::vector<vertex_t> nodes;
    nodes.reserve(neis.size());
    std::transform(range(neis), std::back_inserter(nodes),
                   [this](const auto &vertex) { return get_vertex(vertex); });
    return nodes;
}

template <typename T, typename W>
void Graph<T, W>::add_vertex(vertex_id u, const vertex_value_t &value) {
    vertex_check(false, u, "vertex ", u, " already exists");
    _vertices.emplace(u, Vertex::make_shared(u, value));
}

template <typename T, typename W>
void Graph<T, W>::remove_vertex(vertex_id u) {
    _vertices.erase(u);
}

template <typename T, typename W>
bool Graph<T, W>::adjacent(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->adjacent(_vertices.at(to));
}

template <typename T, typename W>
edge_t<T, W> Graph<T, W>::edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    auto e = _vertices.at(from)->edge(_vertices.at(to));
    if (not e)
        throw std::runtime_error("no edge between " + std::to_string(from) +
                                 " and " + std::to_string(to));
    return get_edge(e);
}

template <typename T, typename W>
const edge_weight_t<T, W> &Graph<T, W>::weight(vertex_id from,
                                               vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->edge(_vertices.at(to))->weight();
}

template <typename T, typename W>
void Graph<T, W>::weight(vertex_id from, vertex_id to,
                         const edge_weight_t &wei) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
}

template <typename T, typename W>
void Graph<T, W>::add_directed_edge(vertex_id from, vertex_id to,
                                    const edge_weight_t &wei) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->add_edge(_vertices.at(to), wei);
}

template <typename T, typename W>
void Graph<T, W>::add_edge(vertex_id from, vertex_id to,
                           const edge_weight_t &wei) const {
    add_edge(from, to, wei, wei);
}

template <typename T, typename W>
void Graph<T, W>::add_edge(vertex_id from, vertex_id to,
                           const edge_weight_t &wei,
                           const edge_weight_t &re_wei) const {
    add_directed_edge(from, to, wei);
    add_directed_edge(to, from, re_wei);
}

template <typename T, typename W>
void Graph<T, W>::remove_edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_edge(_vertices.at(to));
}

template <typename T, typename W>
vertex_t<T, W> Graph<T, W>::get_vertex(vertex_wptr v) const {
    return std::make_pair(v->identity(), v->value());
}

template <typename T, typename W>
edge_t<T, W> Graph<T, W>::get_edge(edge_wptr e) const {
    return std::make_tuple(e->from(), e->to(), e->weight());
}

template <typename T, typename W>
std::pair<vertex_id<T, W>, vertex_id<T, W>>
Graph<T, W>::generate_edge(std::size_t limit) {
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

template <typename T, typename W>
template <typename... Args>
void Graph<T, W>::vertex_check(bool in, vertex_id id,
                               const Args &... msg) const {
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
