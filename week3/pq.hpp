#ifndef PQ_H
#define PQ_H

#include <functional>
#include <set>
#include <stdexcept>
#include <unordered_map>

using item = std::pair<int, int>;

class PQ {
    struct cmp {
        bool operator()(const item &u, const item &v) const;
    };

    std::unordered_map<int, item> items;
    std::set<std::reference_wrapper<item>, cmp> pq;

  public:
    PQ() = default;

    void push(int u, int priority = 0);
    void change_priority(int u, int priority);
    void push(item pair);
    void change_priority(item pair);
    void pop();
    item top() const;
    const item &retrieve(int u) const;
    bool contains(int u) const noexcept;
    bool empty() const noexcept;
    std::size_t size() const noexcept;

  private:
    class iterator {
        using iterator_type = decltype(pq)::iterator;
        iterator_type itr;

        iterator &advance(int incr);

      public:
        explicit iterator(iterator_type itr);

        item operator*() const noexcept;
        item operator->() const noexcept;
        bool operator==(const iterator &rhs) const;
        bool operator!=(const iterator &rhs) const;
        iterator &operator+=(const int &incr);
        iterator &operator-=(const int &incr);
        iterator operator++();
        iterator operator--();
        iterator &operator++(int);
        iterator &operator--(int);
    };

  public:
    iterator begin() const;
    iterator end() const;

    static void unit_testing() noexcept;
};

#endif /* PQ_H */
