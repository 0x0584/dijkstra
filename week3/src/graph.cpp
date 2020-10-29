#include "graph.hpp"

template <typename T, typename U>
Graph<T, U>::Graph(Graph &&g) noexcept : _vertices(std::exchange(g._vertices, {})) {}

template <typename T, typename U> auto Graph<T, U>::vertices() const {
    std::vector<vertex_t> nodes;
    nodes.reverse(_vertices.size());
    std::transform(range(_vertices), std::back_inserter(nodes),
                   [this](auto pair) { return get_vertex(pair->second); });
    return nodes;
}

template <typename T, typename U> auto Graph<T, U>::edges() const {
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

template <typename T, typename U> auto Graph<T, U>::vertex(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return get_vertex(_vertices.at(u));
}

template <typename T, typename U>
void Graph<T, U>::value(vertex_id u, vertex_value_t val) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    _vertices.at(u)->value(val);
}

template <typename T, typename U> auto Graph<T, U>::value(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->value();
}

template <typename T, typename U> auto Graph<T, U>::edges(vertex_id u) const {
    auto v_edges = _vertices.at(u)->edges();
    std::vector<edge_t> links;
    links.reserve(v_edges.size());
    std::transform(range(v_edges), std::back_inserter(links),
                   [this](const auto &edge) { return get_edge(edge); });
    return links;
}

template <typename T, typename U>
auto Graph<T, U>::neighbors(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    auto neis = _vertices.at(u)->neighbors();
    std::vector<vertex_t> nodes;
    nodes.reserve(neis.size());
    std::transform(range(neis), std::back_inserter(nodes),
                   [this](const auto &vertex) { return get_vertex(vertex); });
    return nodes;
}

template <typename T, typename U>
void Graph<T, U>::add_vertex(vertex_id u, vertex_value_t value) {
    vertex_check(false, u, "vertex ", u, " already exists");
    _vertices.emplace(u, Vertex::make_shared(u, value));
}

template <typename T, typename U> void Graph<T, U>::remove_vertex(vertex_id u) {
    _vertices.erase(u);
}

template <typename T, typename U>
bool Graph<T, U>::adjacent(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->adjacent(_vertices.at(to));
}

template <typename T, typename U>
auto Graph<T, U>::edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    auto e = _vertices.at(from)->edge(_vertices.at(to));
    if (not e)
        throw std::runtime_error("no edge between " + std::to_string(from) +
                                 " and " + std::to_string(to));
    return get_edge(e);
}

template <typename T, typename U>
auto Graph<T, U>::weight(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->edge(_vertices.at(to))->weight();
}

template <typename T, typename U>
void Graph<T, U>::weight(vertex_id from, vertex_id to,
                         edge_weight_t wei) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
}

template <typename T, typename U>
void Graph<T, U>::add_directed_edge(vertex_id from, vertex_id to,
                                    edge_weight_t wei) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->add_edge(_vertices.at(to), wei);
}

template <typename T, typename U>
void Graph<T, U>::add_edge(vertex_id from, vertex_id to,
                           edge_weight_t wei) const {
    add_edge(from, to, wei, wei);
}

template <typename T, typename U>
void Graph<T, U>::add_edge(vertex_id from, vertex_id to, edge_weight_t wei,
                           edge_weight_t re_wei) const {
    add_directed_edge(from, to, wei);
    add_directed_edge(to, from, re_wei);
}

template <typename T, typename U>
void Graph<T, U>::remove_edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_edge(_vertices.at(to));
}

/////////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
typename Graph<T, U>::vertex_t Graph<T, U>::get_vertex(vertex_ptr v) const {
    return std::make_pair(v->identity(), v->value());
}

template <typename T, typename U>
typename Graph<T, U>::edge_t Graph<T, U>::get_edge(edge_ptr e) const {
    return std::make_tuple(e->from(), e->to(), e->weight());
}

template <typename T, typename U>
template <typename... Args>
void Graph<T, U>::vertex_check(bool in, vertex_id id,
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
