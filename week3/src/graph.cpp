#include "graph.hpp"

graph::graph() {}
graph::~graph() { v.clear(); }

std::vector<class vertex> graph::vertices() const {
    std::vector<vertex> vertices;
    std::transform(range(v), std::back_inserter(vertices),
                   [](map::value_type pair) { return *pair.second; });
    vertices.shrink_to_fit();
    return vertices;
}

std::vector<class edge> graph::edges() const {
    std::vector<edge> edges;
    std::for_each(range(v), [&edges](map::value_type pair) {
        std::vector<edge_t> v_edges = pair.second->edges();
        return std::transform(range(v_edges), std::back_inserter(edges),
                              [](edge_t e) { return *e; });
    });
    edges.shrink_to_fit();
    return edges;
}

vertex graph::get_vertex(int u) {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    return *v[u];
}

std::vector<vertex> graph::neighbors(int u) {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    std::vector<vertex_t> neis = v[u]->neighbors();
    std::vector<vertex> vec;
    vec.reserve(neis.size());
    std::transform(range(neis), std::back_inserter(vec),
                   [](vertex_t v) { return *v; });
    return vec;
}

std::vector<edge> graph::edges(int u) {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    std::vector<edge_t> es = v[u]->edges();
    std::vector<edge> vec;
    vec.reserve(es.size());
    std::transform(range(es), std::back_inserter(vec),
                   [](edge_t e) { return *e; });
    return vec;
}

void graph::add_vertex(int u, int value) {
    vertex_check(false, u, "vertex " + std::to_string(u) + " already exists");
    v[u] = new vertex{u, value};
}

void graph::set_vertex_value(int u, int value) {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    v[u]->set_value(value);
}

int graph::vertex_value(int u) {
    vertex_check(true, u, "vertex " + std::to_string(u) + " is not found");
    return v[u]->value();
}

void graph::remove_vertex(int u) { v.erase(u); }

bool graph::adjacent(int s, int t) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    return v[s]->adjacent(v[t]);
}

edge graph::get_edge(int s, int t) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    edge_t e = v[s]->edge_to(v[t]);
    if (not e)
        throw std::runtime_error("no edge between " + std::to_string(s) +
                                 " and " + std::to_string(t));
    return *e;
}

void graph::add_directed_edge(int s, int t, int w) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    v[s]->add_edge(v[t], w);
}

void graph::add_edge(int s, int t, int w) { add_edge(s, t, w, w); }

void graph::add_edge(int s, int t, int w, int re_w) {
    add_directed_edge(s, t, w);
    add_directed_edge(t, s, re_w);
}

void graph::remove_edge(int s, int t) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    v[s]->remove_edge(v[t]);
}

int graph::edge_weight(int s, int t) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    return v[s]->edge_to(v[t])->weight();
}

void graph::set_edge_weight(int s, int t, int w) {
    vertex_check(true, s, "vertex " + std::to_string(s) + " is not found");
    vertex_check(true, t, "vertex " + std::to_string(t) + " is not found");
    v[s]->edge_to(v[t])->set_weight(w);
}

/////////////////////////////////////////////////////////////////////////////

void graph::vertex_check(bool in, int id, const std::string &msg) {
    auto itr = v.find(id);
    if (in) {
        if (itr == std::end(v))
            throw std::runtime_error(msg);
    } else {
        if (itr != std::end(v))
            throw std::runtime_error(msg);
    }
}
