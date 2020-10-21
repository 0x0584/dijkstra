#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

typedef class vertex *vertex_t;
typedef class edge *edge_t;

class edge {
    vertex_t s, t;
    int w;

  public:
    edge(vertex_t from, vertex_t to, int weight) : s(from), t(to), w(weight) {}

    vertex_t from() const { return s; }
    vertex_t to() const { return t; }

    constexpr int weight() const { return w; }
    constexpr void set_weight(int weight) { w = weight; }
};

class vertex {
    int id, val;
    std::vector<edge_t> e, re;

  public:
    vertex(int id, int val) : id(id), val(val) {}
    ~vertex() {
        e.clear();
        re.clear();
    }

    constexpr int identity() const { return id; }
    constexpr int value() const { return val; }
    constexpr void set_value(int value) { val = value; }

    const std::vector<edge_t> &edges() const { return e; }
    const std::vector<edge_t> &residuals() const { return re; }

    bool adjacent(vertex_t v) const {
        if (v == nullptr)
            return false;
        return std::any_of(std::begin(e), std::end(e),
                           [&v](auto edge) { return edge->to()->id == v->id; });
    }

    bool add_edge(vertex_t v, int w = 1) {
        if (v == nullptr)
            return false;
        e.push_back(new edge{this, v, w});
        return true;
    }
};

class graph {};

int main(int, char *[]) {
    ;
    return 0;
}
