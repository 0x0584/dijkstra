#ifndef PQ_H
#define PQ_H

#include <functional>
#include <iterator>
#include <set>
#include <stdexcept>
#include <unordered_map>

class PQ {
    struct Item {
        struct cmp {
            bool operator()(const Item &u, const Item &v) const {
                return u.priority() > v.priority();
            }
        };

        Item(int id, int priority) : _id(id), _priority(priority) {}

        int id() const { return _id; }
        int priority() const { return _priority; }
        void priority(int priority) { _priority = priority; }

      private:
        int _id, _priority;
    };

    std::unordered_map<int, Item> items;
    std::set<std::reference_wrapper<Item>, Item::cmp> pq;

  public:
    PQ() = default;

    void push(int u, int priority = 0);
    void push(std::pair<int, int> pair);
    void change_priority(int u, int priority);
    void change_priority(std::pair<int, int> pair);
    void pop();
    int top() const;
    bool contains(int u) const noexcept;
    bool empty() const noexcept;
    std::size_t size() const noexcept;

  private:
    class iterator {
        using iterator_type = decltype(pq)::iterator;
        iterator_type itr;

        iterator &advance(int incr) { return std::advance(itr, incr), *this; }

      public:
        explicit iterator(iterator_type itr) : itr(itr) {}

        int operator*() const { return itr->get().id(); }
        int operator->() const { return itr->get().id(); }

        bool operator==(const iterator &rhs) const { return itr == rhs.itr; }
        bool operator!=(const iterator &rhs) const { return itr != rhs.itr; }

        iterator &operator+=(const int &incr) { return advance(incr); }
        iterator &operator-=(const int &incr) { return advance(incr); }
        iterator operator++() { return advance(1); }
        iterator operator--() { return advance(1); }
        iterator &operator++(int) { return advance(1); }
        iterator &operator--(int) { return advance(1); }
    };

  public:
    iterator begin() const { return iterator(std::begin(pq)); }
    iterator end() const { return iterator(std::end(pq)); }

    static void unit_testing() noexcept;
};

#endif /* PQ_H */
