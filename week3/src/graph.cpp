#include "graph.hpp"

Graph::Graph(Graph &&g) noexcept : _vertices(std::exchange(g._vertices, {})) {}

Graph::Graph() {}
Graph::~Graph() { _vertices.clear(); }

std::vector<Vertex> Graph::vertices() const {
    std::vector<Vertex> vertices;
    std::transform(range(_vertices), std::back_inserter(vertices),
                   [](const auto &pair) { return *pair.second; });
    vertices.shrink_to_fit();
    return vertices;
}

std::vector<Edge> Graph::edges() const {
    std::vector<Edge> edges;
    std::for_each(range(_vertices), [&edges](const auto &pair) {
        std::vector<edge_t> v_edges = pair.second->edges();
        return std::transform(range(v_edges), std::back_inserter(edges),
                              [](const auto &edge) { return *edge; });
    });
    edges.shrink_to_fit();
    return edges;
}

Vertex Graph::vertex(int u) const {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    return *_vertices.at(u);
}

std::vector<Vertex> Graph::neighbors(int u) const {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    std::vector<vertex_t> neis = _vertices.at(u)->neighbors();
    std::vector<Vertex> vec;
    vec.reserve(neis.size());
    std::transform(range(neis), std::back_inserter(vec),
                   [](const auto &vertex) { return *vertex; });
    return vec;
}

std::vector<Edge> Graph::edges(int u) const {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    std::vector<edge_t> es = _vertices.at(u)->edges();
    std::vector<Edge> vec;
    vec.reserve(es.size());
    std::transform(range(es), std::back_inserter(vec),
                   [](const auto &edge) { return *edge; });
    return vec;
}

void Graph::add_vertex(int u, int value) {
    vertex_check(false, u, "vertex " + std::to_string(u) + " already exists");
    _vertices.emplace(u, new Vertex{u, value});
}

void Graph::value(int u, int value) const {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    _vertices.at(u)->value(value);
}

int Graph::value(int u) const {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    return _vertices.at(u)->value();
}

void Graph::remove_vertex(int u) { _vertices.erase(u); }

bool Graph::adjacent(int s, int t) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    return _vertices.at(s)->adjacent(_vertices.at(t));
}

Edge Graph::edge(int s, int t) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    edge_t e = _vertices.at(s)->edge(_vertices.at(t));
    if (not e)
        throw std::runtime_error("no edge between " + std::to_string(s) +
                                 " and " + std::to_string(t));
    return *e;
}

void Graph::add_directed_edge(int s, int t, int w) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    _vertices.at(s)->add_edge(_vertices.at(t), w);
}

void Graph::add_edge(int s, int t, int w) const { add_edge(s, t, w, w); }

void Graph::add_edge(int s, int t, int w, int re_w) const {
    add_directed_edge(s, t, w);
    add_directed_edge(t, s, re_w);
}

void Graph::remove_edge(int s, int t) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    _vertices.at(s)->remove_edge(_vertices.at(t));
}

int Graph::weight(int s, int t) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    return _vertices.at(s)->edge(_vertices.at(t))->weight();
}

void Graph::weight(int s, int t, int w) const {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    _vertices.at(s)->edge(_vertices.at(t))->weight(w);
}

/////////////////////////////////////////////////////////////////////////////

void Graph::vertex_check(bool in, int id, const std::string &msg) const {
    auto itr = _vertices.find(id);
    if (in) {
        if (itr == std::end(_vertices))
            throw std::runtime_error(msg);
    } else {
        if (itr != std::end(_vertices))
            throw std::runtime_error(msg);
    }
}
