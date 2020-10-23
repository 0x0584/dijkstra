#include "graph.hpp"

graph::graph() {}
graph::~graph() { v.clear(); }

std::vector<class vertex> graph::vertices() const {
    std::vector<vertex> vertices;
    std::transform(std::begin(v), std::end(v), std::back_inserter(vertices),
                   [](auto itr) { return *itr.second; });
    vertices.shrink_to_fit();
    return vertices;
}

std::vector<class edge> graph::edges() const {
    std::vector<edge> edges;
    for (auto [_, vi] : v)
        std::transform(std::begin(vi->edges()), std::end(vi->edges()),
                       std::back_inserter(edges),
                       [](auto edges) { return *edges; });
    edges.shrink_to_fit();
    return edges;
}

vertex graph::get_vertex(int u) {
    vertex_check(true, u, "Vertex not found");
    return *v[u];
}

void graph::add_vertex(int u, int value) {
    vertex_check(false, u, "Vertex already exists");
    v[u] = new vertex{u, value};
}

void graph::set_vertex_value(int u, int value) {
    vertex_check(true, u, "Vertex not found");
    v[u]->set_value(value);
}

int graph::vertex_value(int u) {
    vertex_check(true, u, "Vertex not found");
    return v[u]->value();
}

void graph::remove_vertex(int u) { v.erase(u); }

bool graph::adjacent(int from, int to) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    return v[from]->adjacent(v[to]);
}

edge graph::get_edge(int from, int to) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    if (edge_t e = v[from]->edge_to(v[to]); not e)
        throw std::runtime_error("Edge not found");
    else
        return *e;
}

void graph::add_directed_edge(int from, int to, int w) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    v[from]->add_edge(v[to], w);
}

void graph::add_edge(int from, int to, int w) { add_edge(from, to, w, w); }

void graph::add_edge(int from, int to, int w, int re_w) {
    add_directed_edge(from, to, w);
    add_directed_edge(to, from, re_w);
}

void graph::remove_edge(int from, int to) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    v[from]->remove_edge(v[to]);
}

int graph::edge_weight(int from, int to) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    return v[from]->edge_to(v[to])->weight();
}

void graph::set_edge_weight(int from, int to, int w) {
    vertex_check(true, from, "Vertex not found");
    vertex_check(true, to, "Vertex not found");
    v[from]->edge_to(v[to])->set_weight(w);
}

/////////////////////////////////////////////////////////////////////////////

void graph::vertex_check(bool in, int id, std::string_view msg) {
    auto itr = v.find(id);
    if (in) {
        if (itr == std::end(v))
            throw std::runtime_error(msg.data());
    } else {
        if (itr != std::end(v))
            throw std::runtime_error(msg.data());
    }
}
