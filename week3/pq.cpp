#include <iostream>
#include <random>

#include "pq.hpp"

bool PQ::cmp::operator()(const item &u, const item &v) const {
    return u.second < v.second;
}

PQ::iterator &PQ::iterator::advance(int incr) {
    return std::advance(itr, incr), *this;
}
PQ::iterator::iterator(iterator_type itr) : itr(itr) {}
item PQ::iterator::operator*() const noexcept { return itr->get(); }
item PQ::iterator::operator->() const noexcept { return itr->get(); }
bool PQ::iterator::operator==(const iterator &rhs) const {
    return itr == rhs.itr;
}
bool PQ::iterator::operator!=(const iterator &rhs) const {
    return itr != rhs.itr;
}
PQ::iterator &PQ::iterator::operator+=(const int &incr) {
    return advance(incr);
}
PQ::iterator &PQ::iterator::operator-=(const int &incr) {
    return advance(incr);
}
PQ::iterator PQ::iterator::operator++() { return advance(1); }
PQ::iterator PQ::iterator::operator--() { return advance(1); }
PQ::iterator &PQ::iterator::operator++(int) { return advance(1); }
PQ::iterator &PQ::iterator::operator--(int) { return advance(1); }

void PQ::push(int u, int priority) {
    if (not items.emplace(u, std::make_pair(u, priority)).second)
        throw std::runtime_error(std::to_string(u) + " already exists");
    pq.emplace(items.at(u));
}

void PQ::change_priority(int u, int priority) {
    if (items.find(u) == std::end(items))
        throw std::out_of_range(std::to_string(u) + " doesn't exist");
    auto &item = items.at(u);
    pq.erase(item);
    item.second = priority;
    pq.emplace(items.at(u));
}

void PQ::push(std::pair<int, int> pair) { push(pair.first, pair.second); }

void PQ::change_priority(std::pair<int, int> pair) {
    change_priority(pair.first, pair.second);
}

void PQ::pop() {
    if (empty())
        throw std::runtime_error("Empty priority queue");
    auto item = std::begin(pq)->get();
    pq.erase(std::begin(pq));
    items.erase(item.first);
}

bool PQ::contains(int u) const noexcept {
    return items.find(u) != std::end(items);
}

item PQ::top() const {
    if (empty())
        throw std::runtime_error("Empty priority queue");
    return std::begin(pq)->get();
}

const item &PQ::retrieve(int u) const {
    if (not contains(u))
        throw std::out_of_range(std::to_string(u) + " doesn't exist");
    return items.at(u);
}

bool PQ::empty() const noexcept { return not size(); }

std::size_t PQ::size() const noexcept { return pq.size(); }

PQ::iterator PQ::begin() const { return iterator(std::begin(pq)); }
PQ::iterator PQ::end() const { return iterator(std::end(pq)); }

std::ostream &operator<<(std::ostream &os, const item &e) {
    return os << "(" << e.first << " " << e.second << ")";
}

void PQ::unit_testing() noexcept {

    auto output = [](const std::string &msg, const PQ &Cont) {
        std::cout << "---- " << msg << " -----" << std::endl;
        for (const auto e : Cont)
            std::cout << e << "\n";
        std::cout << "--------" << std::endl;
    };

    PQ pq;

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

int main(int, char const *[]) {
    PQ::unit_testing();
    return 0;
}
