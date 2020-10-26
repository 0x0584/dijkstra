#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>

template <typename U, typename V>
auto operator<<(std::ostream &os, const std::pair<U, V> &pair)
    -> std::ostream & {
    return os << "(" << pair.first << " " << pair.second << ")";
}

template <typename T> using ref_t = std::reference_wrapper<T>;

constexpr auto size = 10;

int main(int, char *[]) {
    using item = int;
    using priority = int;

    constexpr auto next_random = []() {
        // std::random_device gen;
        static std::knuth_b gen;
        static std::uniform_int_distribution<int> u(0, size);
        return u(gen);
    };

    constexpr auto output = [](const auto &msg, const auto &Cont) {
        std::cout << msg;
        for (const auto &e : Cont)
            std::cout << e << " ";
        std::cout << std::endl;
    };

    std::vector<std::pair<item, priority>> base;
    std::map<ref_t<item>, ref_t<priority>> items;
    std::multimap<ref_t<priority>, ref_t<item>> pq;

    for (auto i = 0; i < size; ++i)
        base.emplace_back(next_random(), next_random());
    output("base: ", base);

    for (auto &e : base) {
        items.emplace(e.first, e.second);
        pq.emplace(e.second, e.first);
    }
    output("items: ", items);
    output("pq: ", pq);

    for (auto limit = next_random() + 1, loop = 0; loop < limit; ++loop) {
        std::cout << "-------" << std::endl;
        for (auto &e : base)
            items.at(e.first) += next_random();
        output(std::to_string(loop) + "-item: ", items);
        output(std::to_string(loop) + "-pq: ", pq);
    }

    return 0;
}
