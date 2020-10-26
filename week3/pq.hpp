#ifndef PQ_H
#define PQ_H

#include <functional>
#include <map>
#include <stdexcept>

template <typename Item = int> class PQ {
    std::unordered_map<Item, int> items;
    std::multimap<int, std::reference_wrapper<Item>> pq;

  public:
    PQ() {}

    void push(Item u, int priority = 0) {
        auto pair = std::make_pair(priority, u);
        auto pair2 =
            std::make_pair(std::reference_wrapper(pair->second), priority);
        pq.emplace(priority, u);
    }

    void change_priority(Item u, int priority) {}

    void pop() {}

    bool contains(Item u) const { return items.find(u) != std::end(items); }

    Item &top() const {
        if (not size())
            throw std::runtime_error("PQ is empty");
        return pq.begin()->second;
    }

    size_t size() const { return pq.size(); }
};

#endif /* PQ_H */
