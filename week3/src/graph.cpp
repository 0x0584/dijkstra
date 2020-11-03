#include "graph.hpp"

Graph::Graph(Graph &&other) noexcept
    : _vertices(std::exchange(other._vertices, {})) {}

Graph::Graph(int n_vertices, double edge_density) {
    if (edge_density > 1)
        throw std::runtime_error("edge_density > 1");
    const unsigned seed =
        std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_int_distribution<int> verts(1, n_vertices);
    std::uniform_int_distribution<int> vals(1, 500);

    int n_edges = edge_density * ((n_vertices * (n_vertices - 1)) / 2);
    for (int i = 1; i <= n_vertices; ++i)
        add_vertex(i, vals(gen));

    for (int i = 0; i < n_edges; ++i) {
        vertex_id u, v;
        do {
            u = verts(gen), v = verts(gen);
        } while (v == u or adjacent(u, v));
        add_edge({u, v}, vals(gen));
    }
    std::cout << "\n";
}

Graph::Graph(
    const std::vector<std::pair<vertex_id, vertex_value_t>> &vertices,
    const std::vector<std::pair<std::pair<vertex_id, vertex_id>, edge_weight_t>>
        &edges,
    bool directed) {

    for (const auto &v : vertices)
        add_vertex(v.first, v.second);
    for (const auto &e : edges) {
        if (directed)
            add_directed_edge(e.first, e.second);
        else
            add_edge(e.first, e.second);
    }
}

Graph &Graph::operator=(Graph &&other) noexcept {
    if (this == &other)
        return *this;
    _vertices.clear();
    _vertices = std::exchange(other._vertices, {});
    return *this;
}

std::vector<vertex_id> Graph::vertices() const {
    std::vector<vertex_id> nodes;
    nodes.reserve(_vertices.size());
    std::transform(itr_range(_vertices), std::back_inserter(nodes),
                   [](const auto &pair) { return pair.second->identity(); });
    return nodes;
}

std::vector<std::pair<vertex_id, vertex_id>> Graph::edges() const {
    std::vector<std::pair<vertex_id, vertex_id>> links;
    std::for_each(itr_range(_vertices), [&links](const auto &pair) {
        const auto &v_edges = pair.second->edges();
        return std::transform(itr_range(v_edges), std::back_inserter(links),
                              [](const auto &e) { return e; });
    });
    links.shrink_to_fit();
    return links;
}

bool Graph::has_vertex(vertex_id u) const {
    return _vertices.find(u) != std::end(_vertices);
}

const vertex_value_t &Graph::value(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->value();
}

void Graph::value(vertex_id u, const vertex_value_t &val) {
    vertex_check(true, u, "vertex ", u, " is not found");
    _vertices.at(u)->value(val);
}

std::vector<std::pair<vertex_id, vertex_id>> Graph::edges(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->edges();
}

std::vector<vertex_id> Graph::neighbors(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->neighbors();
}

void Graph::add_vertex(vertex_id u, const vertex_value_t &val) {
    vertex_check(false, u, "vertex ", u, " already exists");
    _vertices.emplace(u, std::make_shared<Vertex>(u, val));
}

void Graph::remove_vertex(vertex_id u) {
    vertex_check(true, u, "vertex ", u, " is not found");
    _vertices.erase(u);
}

bool Graph::adjacent(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->adjacent(_vertices.at(to));
}

bool Graph::adjacent(std::pair<vertex_id, vertex_id> e) const {
    return adjacent(e.first, e.second);
}

const edge_weight_t &Graph::weight(std::pair<vertex_id, vertex_id> e) const {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->edge(_vertices.at(to))->weight();
}

void Graph::weight(std::pair<vertex_id, vertex_id> e,
                   const edge_weight_t &wei) {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
}

void Graph::add_directed_edge(std::pair<vertex_id, vertex_id> e,
                              const edge_weight_t &wei) const {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->add_directed_edge(_vertices.at(to), wei);
}

void Graph::add_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei) const {
    add_edge(e, wei, wei);
}

void Graph::add_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei,
                     const edge_weight_t &re_wei) const {
    add_directed_edge(e, wei);
    add_directed_edge({e.second, e.first}, re_wei);
}

void Graph::remove_edge(std::pair<vertex_id, vertex_id> e) const {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_edge(_vertices.at(to));
}

void Graph::remove_directed_edge(std::pair<vertex_id, vertex_id> e) const {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_directed_edge(_vertices.at(to));
}

void Graph::unit_testing() noexcept {}
