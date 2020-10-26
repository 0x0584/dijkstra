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

    std::unordered_map<item, priority> items;
    std::multimap<ref_t<priority>, ref_t<item>> pq;
    std::vector<std::pair<item, priority>> base;

    for (auto i = 0; i < size; ++i)
        base.emplace_back(i, next_random());
    output("vector: ", base);

    for (auto &e : base)
        items.emplace(e.first, e.second);
    output("items: ", items);

    for (auto &e : items) {
        using item_t = std::remove_const_t<decltype(e.first)>;
        using item_ref = std::add_lvalue_reference_t<item_t>;
        pq.emplace(ref_t<decltype(e.second)>(e.second),
                   ref_t<item_t>(const_cast<item_ref>(e.first)));
    }
    output("pq: ", pq);

    for (auto limit = next_random() + 1, loop = 0; loop < limit; ++loop) {
        items.at(base[next_random()].first) += next_random();
        output(std::to_string(loop) + ": ", pq);
        output(std::to_string(loop) + ": ", items);
        std::cout << "-------" << std::endl;
    }

    return 0;
}
