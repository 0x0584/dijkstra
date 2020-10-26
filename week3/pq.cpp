#include <iostream>
#include <random>

#include "pq.hpp"

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

void PQ::unit_testing() noexcept {
    constexpr auto limit = 0x0584;
    auto next_random = []() {
        // static std::knuth_b gen;
        static std::random_device gen;
        static std::uniform_int_distribution<int> u(0, limit);
        return u(gen);
    };

    PQ pq;

    for (auto to = next_random() + 1, loop = 0; loop < to; ++loop) {
        auto u = next_random(), p = next_random();
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
