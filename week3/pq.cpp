#include <iostream>
#include <random>

#include "pq.hpp"

bool PQ::Item::cmp::operator()(const Item &u, const Item &v) const {
    return u.priority() < v.priority();
}
PQ::Item::Item(int id, int priority) : _id(id), _priority(priority) {}
int PQ::Item::id() const { return _id; }
int PQ::Item::priority() const { return _priority; }
void PQ::Item::priority(int priority) { _priority = priority; }

PQ::iterator &PQ::iterator::advance(int incr) {
    return std::advance(itr, incr), *this;
}
PQ::iterator::iterator(iterator_type itr) : itr(itr) {}
int PQ::iterator::operator*() const { return itr->get().id(); }
int PQ::iterator::operator->() const { return itr->get().id(); }
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
    if (not items.emplace(u, Item{u, priority}).second)
        throw std::runtime_error(std::to_string(u) + " already exists");
    pq.emplace(items.at(u));
}

void PQ::change_priority(int u, int priority) {
    if (items.find(u) == std::end(items))
        throw std::runtime_error(std::to_string(u) + " doesn't exist");
    auto &item = items.at(u);
    pq.erase(item);
    item.priority(priority);
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
    items.erase(item.id());
}

bool PQ::contains(int u) const noexcept {
    return items.find(u) != std::end(items);
}

int PQ::top() const {
    if (empty())
        throw std::runtime_error("Empty priority queue");
    return std::begin(pq)->get().id();
}

bool PQ::empty() const noexcept { return not size(); }

std::size_t PQ::size() const noexcept { return pq.size(); }

PQ::iterator PQ::begin() const { return iterator(std::begin(pq)); }
PQ::iterator PQ::end() const { return iterator(std::end(pq)); }

void PQ::unit_testing() noexcept {
    PQ pq;

    for (auto to = 10, loop = 0; loop < to; ++loop) {
        auto u = loop, p = to - loop;
        std::cout << "u: " << u << " p: " << p << "\n";
        try {
            pq.push(u, p);
        } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << "--------" << std::endl;
    for (const auto e : pq)
        std::cout << e << "\n";
}

int main(int, char const *[]) {
    PQ::unit_testing();
    return 0;
}
