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
            bool operator()(const Item &u, const Item &v) const;
        };

        Item(int id, int priority);

        int id() const;
        int priority() const;
        void priority(int priority);

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

        iterator &advance(int incr);

      public:
        explicit iterator(iterator_type itr);

        int operator*() const;
        int operator->() const;

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
