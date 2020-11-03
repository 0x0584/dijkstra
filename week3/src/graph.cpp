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
        add_edge(u, v, vals(gen));
    }
    std::cout << "\n";
}

Graph::Graph(const std::vector<vertex_t> &vertices,
             const std::vector<edge_t> &edges, bool directed) {
    using std::get;

    for (const auto &v : vertices)
        add_vertex(v.first, v.second);
    for (const auto &e : edges) {
        if (directed)
            add_directed_edge(get<0>(e), get<1>(e), get<2>(e));
        else
            add_edge(get<0>(e), get<1>(e), get<2>(e));
    }
}

Graph &Graph::operator=(Graph &&other) noexcept {
    if (this == &other)
        return *this;
    _vertices.clear();
    _vertices = std::exchange(other._vertices, {});
    return *this;
}

std::vector<Graph::vertex_t> Graph::vertices() const {
    std::vector<vertex_t> nodes;
    const auto resolve = [](const auto &pair) {
        return Vertex::make(pair.second);
    };
    nodes.reserve(_vertices.size());
    std::transform(itr_range(_vertices), std::back_inserter(nodes), resolve);
    return nodes;
}

std::vector<Graph::edge_t> Graph::edges() const {
    std::vector<edge_t> links;
    const auto resolve = [&links](const auto &pair) {
        const auto &v_edges = pair.second->edges();
        return std::transform(itr_range(v_edges), std::back_inserter(links),
                              [](auto e) { return Edge::make(e); });
    };
    std::for_each(itr_range(_vertices), resolve);
    links.shrink_to_fit();
    return links;
}

bool Graph::has_vertex(vertex_id u) const {
    return _vertices.find(u) != std::end(_vertices);
}

Graph::vertex_t Graph::vertex(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return Vertex::make(_vertices.at(u));
}

const Graph::vertex_value_t &Graph::value(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    return _vertices.at(u)->value();
}

void Graph::value(vertex_id u, const vertex_value_t &val) {
    vertex_check(true, u, "vertex ", u, " is not found");
    _vertices.at(u)->value(val);
}

std::vector<Graph::edge_t> Graph::edges(vertex_id u) const {
    std::vector<edge_t> links;
    auto v_edges = _vertices.at(u)->edges();
    links.reserve(v_edges.size());
    std::transform(itr_range(v_edges), std::back_inserter(links),
                   [](auto edge) { return Edge::make(edge); });
    return links;
}

std::vector<Graph::vertex_t> Graph::neighbors(vertex_id u) const {
    vertex_check(true, u, "vertex ", u, " is not found");
    std::vector<vertex_t> nodes;
    auto neis = _vertices.at(u)->neighbors();
    nodes.reserve(neis.size());
    std::transform(itr_range(neis), std::back_inserter(nodes),
                   [](auto vertex) { return Vertex::make(vertex); });
    return nodes;
}

void Graph::add_vertex(vertex_id u, const vertex_value_t &val) {
    vertex_check(false, u, "vertex ", u, " already exists");
    _vertices.emplace(u, std::make_shared<Vertex>(u, val));
}

void Graph::remove_vertex(vertex_id u) { _vertices.erase(u); }

bool Graph::adjacent(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->adjacent(_vertices.at(to));
}

Graph::edge_t Graph::edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    if (not adjacent(from, to))
        throw std::runtime_error("no edge between " + std::to_string(from) +
                                 " and " + std::to_string(to));
    return Edge::make(_vertices.at(from)->edge(_vertices.at(to)));
}

const Graph::edge_weight_t &Graph::weight(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    return _vertices.at(from)->edge(_vertices.at(to))->weight();
}

void Graph::weight(vertex_id from, vertex_id to, const edge_weight_t &wei) {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->edge(_vertices.at(to))->weight(wei);
}

void Graph::add_directed_edge(vertex_id from, vertex_id to,
                              const edge_weight_t &wei) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->add_directed_edge(_vertices.at(to), wei);
}

void Graph::add_edge(vertex_id from, vertex_id to,
                     const edge_weight_t &wei) const {
    add_edge(from, to, wei, wei);
}

void Graph::add_edge(vertex_id from, vertex_id to, const edge_weight_t &wei,
                     const edge_weight_t &re_wei) const {
    add_directed_edge(from, to, wei);
    add_directed_edge(to, from, re_wei);
}

void Graph::remove_edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_edge(_vertices.at(to));
}

void Graph::remove_directed_edge(vertex_id from, vertex_id to) const {
    vertex_check(true, from, "vertex ", from, " is not found");
    vertex_check(true, to, "vertex ", to, " is not found");
    _vertices.at(from)->remove_directed_edge(_vertices.at(to));
}

void Graph::unit_testing() noexcept {
    std::cout << " ----------- Testing Graph ---------------\n";

    int n_verts = 0, n_edges = 0;
    {
        Graph g;
        int n, m;

        std::cin >> n >> m;
        while (n--) {
            int u, v;
            std::cin >> u >> v;
            if (not g.has_vertex(u))
                g.add_vertex(u, u);
            if (not g.has_vertex(v))
                g.add_vertex(v, v);
            g.add_directed_edge(u, v, 1);
        }
        n_verts = g.vertices().size(), n_edges = g.edges().size();
        std::cout << "graph has " << n_verts << " vertices and " << n_edges
                  << " edges\n";
    }
    {
        int k = (n_verts * (n_verts - 1));
        std::cout << "complete graph has " << k << " edges\n";
        std::cout << std::endl;
    }
    for (double d = 0; d <= 1.05; d += 0.05) {
        Graph h{n_verts, std::min(d, 1.0)};
        std::cout << "edge density: " << d << " has " << h.edges().size()
                  << " edges\n";
    }
}
