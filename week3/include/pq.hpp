#ifndef PQ_H
#define PQ_H

#include <functional>
#include <set>
#include <stdexcept>
#include <unordered_map>

template <typename U, typename V = int> class PQ {
    struct cmp {
        bool operator()(const std::pair<U, V> &u,
                        const std::pair<U, V> &v) const {
            return u.second < v.second;
        }
    };

    std::unordered_map<int, std::pair<U, V>> items;
    std::set<std::reference_wrapper<std::pair<U, V>>, cmp> pq;

  public:
    PQ() = default;

    void push(int u, int priority = 0);
    void change_priority(int u, int priority);
    void push(std::pair<U, V> pair);
    void change_priority(std::pair<U, V> pair);
    void pop();
    std::pair<U, V> top() const;
    const std::pair<U, V> &retrieve(int u) const;
    bool contains(int u) const noexcept;
    bool empty() const noexcept;
    std::size_t size() const noexcept;

  private:
    class iterator {
        using iterator_type = typename decltype(pq)::iterator;
        iterator_type itr;

        iterator &advance(int incr);

      public:
        explicit iterator(iterator_type itr);

        const std::pair<U, V> &operator*() const noexcept;
        const std::pair<U, V> &operator->() const noexcept;
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
