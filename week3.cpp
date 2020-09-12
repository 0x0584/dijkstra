#include <iostream>
#include <vector>

using namespace std;

class edge {
    int from, to;
    int weight;

  public:
    edge(int s, int t, int w) : from(s), to(t), weight(w) {}
};

class vertex {
    int id;
    int value;
    vector<edge> edges;

  public:
    vertex(int id, int val) : id(id), value(val) {}
};

class graph {

  public:
    graph(double edge_density, double min_weight, double max_weight) {}

    vector<vertex> vertices() const {}
    vector<edge> edges() const {}

    bool adjacent(int v, int w) const {}
    vector<edge> neighbors(int v) const {}

    void add_edge(int v, int w) {}
    void del_edge(int v, int w) {}

    vertex vertex(int v) {}
};

int main(int, char *[]) { return 0; }
