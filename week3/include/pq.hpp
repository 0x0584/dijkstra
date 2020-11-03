#ifndef PQ_H
#define PQ_H

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

/// \brief Priority Queue Data structure
///
/// It orders items based on their priority, which could be changed.
class PQ {
  public:
    /// \brief each item in the priority queue is a pair of value and priority
    using item = std::pair</* value */ int, /* priority */ int>;

    /// \brief functor for comparing two items by their priority
    struct cmp {
        bool operator()(const item &u, const item &v) const {
            return u.second < v.second;
        }
    };

  private:
    /// \brief keeping items inside a map to guarantee O(lg n) for all
    /// operations
    ///
    /// values should be unique since push() throws if an item is not inserted
    std::map<int, item> items;

    /// \brief keeping the items inside a set to make sure their ordered since
    /// STL sets are ordered using cmp (item's priority). The reference wrapper
    /// is to avoid having duplicates, all items are stored in the items map
    std::set<std::reference_wrapper<item>, cmp> pq;

  public:
    PQ() = default; ///< default constructor

    /// \brief pushing an new unique item to the priority queue, throws if the
    /// items exists beforehand. see contains().
    ///
    /// \param u item's value
    /// \param priority item's priority defaulted to 0 (top priority)
    void push(int u, int priority = 0) {
        // add to items
        if (not items.emplace(u, std::make_pair(u, priority)).second)
            throw std::runtime_error(std::to_string(u) + " already exists");
        pq.emplace(items.at(u)); // then add item to priority queue
    }

    /// \brief Overload for handy usage (probably)
    ///
    /// \param item a pair of {value, priority}
    void push(const item &pair) { push(pair.first, pair.second); }

    /// \brief changes the priority of an existing item, throws if not
    ///
    /// \param u exciting item's value
    /// \param priority new priority
    void change_priority(int u, int priority) {
        if (items.find(u) == std::end(items))
            throw std::out_of_range(std::to_string(u) + " doesn't exist");
        pq.erase(items.at(u));         // remove the item from the set
        items.at(u).second = priority; // do the change
        pq.emplace(items.at(u));       // add it to the set with new priority
    }

    /// \brief Overload for handy usage
    ///
    /// \param item a pair of {value, new_priority}
    void change_priority(const item &pair) {
        change_priority(pair.first, pair.second);
    }

    /// \brief removes the top item from the priority queue, throws if empty
    void pop() {
        if (empty())
            throw std::runtime_error("Empty priority queue");
        auto item = std::begin(pq)->get();
        pq.erase(std::begin(pq));
        items.erase(item.first);
    }

    /// \brief get the item with top priority
    ///
    /// \return item at top
    item top() const {
        if (empty())
            throw std::runtime_error("Empty priority queue");
        return std::begin(pq)->get();
    }

    /// \brief get certain item by its value, throws if the item is not found
    ///
    /// \return the desired item
    const item &retrieve(int u) const {
        if (not contains(u))
            throw std::out_of_range(std::to_string(u) + " doesn't exist");
        return items.at(u);
    }

    /// \return true if item's value is found, false otherwise
    bool contains(int u) const noexcept {
        return items.find(u) != std::end(items);
    }

    /// \return true is empty, false otherwise
    bool empty() const noexcept { return pq.empty(); }

    /// \brief
    int size() const noexcept { return pq.size(); }

    /// \brief an internal iterator implementation to iterate over the priority
    /// queue items. to be used the range for loops
    ///
    /// it can incremented only, and dereferenced using wither `*` or `->`
    class iterator {
        using iterator_type = typename decltype(pq)::iterator;
        iterator_type itr;

        iterator &advance(int incr) { return std::advance(itr, incr), *this; }

      public:
        explicit iterator(iterator_type itr) : itr(itr) {}

        const item &operator*() const noexcept { return itr->get(); }
        const item &operator->() const noexcept { return itr->get(); }
        bool operator==(const iterator &rhs) const { return itr == rhs.itr; }
        bool operator!=(const iterator &rhs) const { return itr != rhs.itr; }
        iterator operator++() { return advance(1); }
        iterator &operator++(int) { return advance(1); }
    };

    /// \brief begin iterator of the priority queue
    ///
    /// \return iterator to the beginning of the priority queue
    iterator begin() const { return iterator(std::begin(pq)); }

    /// \brief end iterator to the end of the priority queue.
    /// it's not the last item, since end() is passed the last item
    ///
    /// \return iterator to the end of the priority queue
    iterator end() const { return iterator(std::end(pq)); }

    /// \brief unit testing for all functions of the class
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
        output("altering priority of even entries", q);

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
