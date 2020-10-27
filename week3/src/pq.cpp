#include <iostream>
#include <random>

#include "pq.hpp"

template <typename U, typename V>
typename PQ<U, V>::iterator &PQ<U, V>::iterator::advance(int incr) {
    return std::advance(itr, incr), *this;
}

template <typename U, typename V>
PQ<U, V>::iterator::iterator(iterator_type itr) : itr(itr) {}

template <typename U, typename V>
const std::pair<U, V> &PQ<U, V>::iterator::operator*() const noexcept {
    return itr->get();
}

template <typename U, typename V>
const std::pair<U, V> &PQ<U, V>::iterator::operator->() const noexcept {
    return itr->get();
}

template <typename U, typename V>
bool PQ<U, V>::iterator::operator==(const iterator &rhs) const {
    return itr == rhs.itr;
}

template <typename U, typename V>
bool PQ<U, V>::iterator::operator!=(const iterator &rhs) const {
    return itr != rhs.itr;
}

template <typename U, typename V>
typename PQ<U, V>::iterator &PQ<U, V>::iterator::operator+=(const int &incr) {
    return advance(incr);
}

template <typename U, typename V>
typename PQ<U, V>::iterator &PQ<U, V>::iterator::operator-=(const int &incr) {
    return advance(incr);
}

template <typename U, typename V>
typename PQ<U, V>::iterator PQ<U, V>::iterator::operator++() {
    return advance(1);
}

template <typename U, typename V>
typename PQ<U, V>::iterator PQ<U, V>::iterator::operator--() {
    return advance(1);
}

template <typename U, typename V>
typename PQ<U, V>::iterator &PQ<U, V>::iterator::operator++(int) {
    return advance(1);
}

template <typename U, typename V>
typename PQ<U, V>::iterator &PQ<U, V>::iterator::operator--(int) {
    return advance(1);
}

template <typename U, typename V> void PQ<U, V>::push(int u, int priority) {
    if (not items.emplace(u, std::make_pair(u, priority)).second)
        throw std::runtime_error(std::to_string(u) + " already exists");
    pq.emplace(items.at(u));
}

template <typename U, typename V>
void PQ<U, V>::change_priority(int u, int priority) {
    if (items.find(u) == std::end(items))
        throw std::out_of_range(std::to_string(u) + " doesn't exist");
    auto &item = items.at(u);
    pq.erase(item);
    item.second = priority;
    pq.emplace(items.at(u));
}

template <typename U, typename V> void PQ<U, V>::push(std::pair<U, V> pair) {
    push(pair.first, pair.second);
}

template <typename U, typename V>
void PQ<U, V>::change_priority(std::pair<U, V> pair) {
    change_priority(pair.first, pair.second);
}

template <typename U, typename V> void PQ<U, V>::pop() {
    if (empty())
        throw std::runtime_error("Empty priority queue");
    auto item = std::begin(pq)->get();
    pq.erase(std::begin(pq));
    items.erase(item.first);
}

template <typename U, typename V>
bool PQ<U, V>::contains(int u) const noexcept {
    return items.find(u) != std::end(items);
}

template <typename U, typename V> std::pair<U, V> PQ<U, V>::top() const {
    if (empty())
        throw std::runtime_error("Empty priority queue");
    return std::begin(pq)->get();
}

template <typename U, typename V>
const std::pair<U, V> &PQ<U, V>::retrieve(int u) const {
    if (not contains(u))
        throw std::out_of_range(std::to_string(u) + " doesn't exist");
    return items.at(u);
}

template <typename U, typename V> bool PQ<U, V>::empty() const noexcept {
    return not size();
}

template <typename U, typename V> std::size_t PQ<U, V>::size() const noexcept {
    return pq.size();
}

template <typename U, typename V>
typename PQ<U, V>::iterator PQ<U, V>::begin() const {
    return iterator(std::begin(pq));
}

template <typename U, typename V>
typename PQ<U, V>::iterator PQ<U, V>::end() const {
    return iterator(std::end(pq));
}

template <typename U, typename V>
std::ostream &operator<<(std::ostream &os, const std::pair<U, V> &e) {
    return os << "(" << e.first << " " << e.second << ")";
}

template <typename U, typename V> void PQ<U, V>::unit_testing() noexcept {
    auto output = [](const std::string &msg, const PQ<U, V> &Cont) {
        std::cout << "---- " << msg << " -----" << std::endl;
        for (const auto e : Cont)
            std::cout << e << "\n";
        std::cout << "--------" << std::endl;
    };

    PQ<U, V> pq;

    for (auto to = 10, loop = 0; loop < to; ++loop) {
        auto u = loop, p = to - loop;
        std::cout << "u: " << u << " p: " << p << "\n";
        try {
            pq.push(u, p);
        } catch (const std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
    }

    output("constructing", pq);
    for (auto to = 10, loop = 0; loop < to; loop += 2)
        pq.change_priority(loop, pq.retrieve(loop).second * 2);
    output("altering priority of even entires", pq);

    for (auto to = 10, loop = 0; loop < to; loop += 2) {
        std::cout << "removing: " << pq.top() << std::endl;
        pq.pop();
    }
    output("priority queue afterwards", pq);
}
