#include "graph.hpp"

#include <iomanip>

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
    for (const auto &[e1, e2] : edges(u))
        remove_directed_edge({e2, e1});
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
    if (not adjacent(from, to))
        throw std::out_of_range("no edge between " + std::to_string(from) +
                                " and " + std::to_string(to));
    return _vertices.at(from)->edge(_vertices.at(to))->weight();
}

void Graph::weight(std::pair<vertex_id, vertex_id> e,
                   const edge_weight_t &wei) {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    if (not adjacent(from, to))
        throw std::out_of_range("no edge between " + std::to_string(from) +
                                " and " + std::to_string(to));
    _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
}

void Graph::add_directed_edge(std::pair<vertex_id, vertex_id> e,
                              const edge_weight_t &wei) {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->add_directed_edge(_vertices.at(to), wei);
}

void Graph::add_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei) {
    add_edge(e, wei, wei);
}

void Graph::add_edge(std::pair<vertex_id, vertex_id> e,
                     const edge_weight_t &wei, const edge_weight_t &re_wei) {
    add_directed_edge(e, wei);
    add_directed_edge({e.second, e.first}, re_wei);
}

void Graph::create_directed_edge(std::pair<vertex_id, vertex_id> e,
                                 const edge_weight_t &wei) {
    auto &&[from, to] = e;
    if (not has_vertex(from))
        add_vertex(from, 0);
    if (not has_vertex(to))
        add_vertex(to, 0);
    _vertices.at(from)->add_directed_edge(_vertices.at(to), wei);
}

void Graph::create_edge(std::pair<vertex_id, vertex_id> e,
                        const edge_weight_t &wei) {
    create_edge(e, wei, wei);
}

void Graph::create_edge(std::pair<vertex_id, vertex_id> e,
                        const edge_weight_t &wei, const edge_weight_t &re_wei) {
    create_directed_edge(e, wei);
    create_directed_edge({e.second, e.first}, re_wei);
}

void Graph::remove_edge(std::pair<vertex_id, vertex_id> e) {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    if (not adjacent(from, to))
        throw std::out_of_range("no edge between " + std::to_string(from) +
                                " and " + std::to_string(to));
    _vertices.at(from)->remove_edge(_vertices.at(to));
}

void Graph::remove_directed_edge(std::pair<vertex_id, vertex_id> e) {
    auto &&[from, to] = e;
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    if (not adjacent(from, to))
        throw std::out_of_range("no edge between " + std::to_string(from) +
                                " and " + std::to_string(to));
    _vertices.at(from)->remove_directed_edge(_vertices.at(to));
}

void Graph::unit_testing() noexcept {
    Graph g;

    const int n_vertices = 10;

    const auto print_graph = [](const Graph &g, const std::string &msg,
                                bool print_verts, bool print_edges) {
        std::cout << "##### " << msg << " #####\n";

        int line_break = 7;
        if (print_verts) {
            auto &&verts = g.vertices();
            std::cout << "vertex set: {\n";
            for (unsigned i = 0; i < verts.size(); ++i)
                std::cout << "  " << verts[i] << " -> " << g.value(verts[i])
                          << (i + 1 == verts.size() ? "" : ",")
                          << ((i and i % line_break == 0 and
                               i + 1 != verts.size())
                                  ? "\n"
                                  : "");
            std::cout << "\n}\n";
            if (print_edges)
                std::cout << '\n';
        }
        if (print_edges) {
            line_break = 4;
            auto &&edges = g.edges();
            std::cout << "edge set: {\n";
            for (unsigned i = 0; i < edges.size(); ++i)
                std::cout << "  {" << edges[i].first << ", " << edges[i].second
                          << "} -> " << g.weight(edges[i])
                          << (i + 1 == edges.size() ? "" : ",")
                          << (i and i % line_break == 0 and
                                      i + 1 != edges.size()
                                  ? "\n"
                                  : "");
            std::cout << "\n}\n";
        }
        if (print_verts or print_edges)
            std::cout << std::endl;
    };

    const auto check_vertices = [](const Graph &g) {
        std::cout << "### checking vertices ###\n";
        for (vertex_id i = 1; i <= n_vertices; ++i)
            std::cout << "has vertex " << i << ": " << std::boolalpha
                      << g.has_vertex(i) << "\n";
    };

    for (vertex_id i = 1; i <= n_vertices; ++i)
        g.add_vertex(i, n_vertices - i + 1);
    print_graph(g, "constructing the graph", true, true);
    check_vertices(g);
    std::cout << std::endl;

    auto &&verts = g.vertices();
    for (unsigned i = 1; i < verts.size(); ++i) {
        auto u = verts[i], v = verts[i - 1];
        g.add_edge({u, v}, g.value(v), g.value(v));
    }
    print_graph(g, "adding undirected edges", false, true);

    for (unsigned i = 0; i < verts.size(); i += 2)
        g.remove_vertex(verts[i]);
    print_graph(g, "removing odd vertices", true, true);
    check_vertices(g);
    std::cout << std::endl;

    verts = g.vertices();
    for (unsigned i = 1; i < verts.size(); i++) {
        auto u = verts[i], v = verts[i - 1];
        g.add_directed_edge({u, v}, g.value(v) + g.value(u));
    }
    print_graph(g, "adding directed edges", false, true);

    for (unsigned i = 1; i < verts.size(); ++i) {
        auto u = verts[i], v = verts[i - 1];
        g.value(u, g.value(v) + g.value(u));
    }
    print_graph(g, "changing values", true, false);

    auto &&_edges = g.edges();
    for (unsigned i = 1; i < _edges.size(); ++i) {
        auto u = _edges[i], v = _edges[i - 1];
        g.weight(u, g.weight(u) + g.weight(v));
    }
    print_graph(g, "changing weights", false, true);

    for (unsigned i = 0; i < verts.size(); ++i) {
        for (unsigned j = 0; j < verts.size(); ++j) {
            auto u = verts[i], v = verts[j];
            if (u == v)
                continue;
            g.add_edge({u, v}, g.value(u) * 2, g.value(v) * 3);
        }
    }
    print_graph(g, "making the graph complete", false, true);

    for (unsigned i = 1; i < verts.size(); i += 2) {
        auto u = verts[i], v = verts[i - 1];
        g.remove_directed_edge({u, v});
    }
    print_graph(g, "removing directed edges", false, true);

    for (unsigned i = 1; i < verts.size(); ++i) {
        auto u = verts[i], v = verts[i - 1];
        if (g.adjacent(u, v))
            g.remove_edge({u, v});
    }
    print_graph(g, "undirected edges", true, true);

    for (vertex_id i = 2; i <= n_vertices; ++i)
        g.create_edge({i, i - 1}, n_vertices - i + 1);
    check_vertices(g);
    std::cout << "\n";
    print_graph(g, "creating edges with the missing vertices", true, true);

    Graph h = std::move(g);
    print_graph(g, "graph is only movable, should be empty", true, true);
    print_graph(h, "graph is only movable, should be old graph", true, true);
}
