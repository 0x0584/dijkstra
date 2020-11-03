#ifndef PQ_H
#define PQ_H

#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_map>

class PQ {
  public:
    using pq_pair = std::pair<int, int>;
    struct cmp {
        bool operator()(const pq_pair &u, const pq_pair &v) const {
            return u.second < v.second;
        }
    };

  private:
    std::unordered_map<int, pq_pair> items;
    std::set<std::reference_wrapper<pq_pair>, cmp> pq;

  public:
    PQ() = default;

    void push(int u, int priority = 0) {
        if (not items.emplace(u, std::make_pair(u, priority)).second)
            throw std::runtime_error(std::to_string(u) + " already exists");
        pq.emplace(items.at(u));
    }

    void push(const pq_pair &pair) { push(pair.first, pair.second); }

    void change_priority(int u, int priority) {
        if (items.find(u) == std::end(items))
            throw std::out_of_range(std::to_string(u) + " doesn't exist");
        auto &item = items.at(u);
        pq.erase(item);
        item.second = priority;
        pq.emplace(items.at(u));
    }

    void change_priority(const pq_pair &pair) {
        change_priority(pair.first, pair.second);
    }

    void pop() {
        if (empty())
            throw std::runtime_error("Empty priority queue");
        auto item = std::begin(pq)->get();
        pq.erase(std::begin(pq));
        items.erase(item.first);
    }

    pq_pair top() const {
        if (empty())
            throw std::runtime_error("Empty priority queue");
        return std::begin(pq)->get();
    }

    const pq_pair &retrieve(int u) const {
        if (not contains(u))
            throw std::out_of_range(std::to_string(u) + " doesn't exist");
        return items.at(u);
    }

    bool contains(int u) const noexcept {
        return items.find(u) != std::end(items);
    }

    bool empty() const noexcept { return not size(); }
    int size() const noexcept { return pq.size(); }

    class iterator {
        using iterator_type = typename decltype(pq)::iterator;
        iterator_type itr;

        iterator &advance(int incr) { return std::advance(itr, incr), *this; }

      public:
        explicit iterator(iterator_type itr) : itr(itr) {}

        const pq_pair &operator*() const noexcept { return itr->get(); }
        const pq_pair &operator->() const noexcept { return itr->get(); }
        bool operator==(const iterator &rhs) const { return itr == rhs.itr; }
        bool operator!=(const iterator &rhs) const { return itr != rhs.itr; }
        iterator operator++() { return advance(1); }
        iterator &operator++(int) { return advance(1); }
    };

    iterator begin() const { return iterator(std::begin(pq)); }
    iterator end() const { return iterator(std::end(pq)); }

    static void unit_testing() noexcept {
        std::cout << " ----------- Testing Priority Queue ---------------\n";
        auto output = [](const std::string &msg, const PQ &Cont) {
            std::cout << "---- " << msg << " -----" << std::endl;
            for (const auto &e : Cont)
                std::cout << e.first << " " << e.second << "\n";
            std::cout << "--------" << std::endl;
        };

        PQ q;

        for (auto to = 10, loop = 0; loop < to; ++loop) {
            auto u = loop, p = to - loop;
            std::cout << "u: " << u << " p: " << p << "\n";
            try {
                q.push(u, p);
            } catch (const std::runtime_error &e) {
                std::cout << e.what() << std::endl;
            }
        }

        output("constructing", q);
        for (auto to = 10, loop = 0; loop < to; loop += 2)
            q.change_priority(loop, q.retrieve(loop).second * 2);
        output("altering priority of even entires", q);

        for (auto to = 10, loop = 0; loop < to; loop += 2) {
            auto &&_top = q.top();
            std::cout << "removing: " << _top.first << " " << _top.second
                      << std::endl;
            q.pop();
        }
        output("priority queue afterwards", q);
    }
};

#endif /* PQ_H */
