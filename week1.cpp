// File: Week1.cpp
// Compile using: g++ week1.cpp -std=c++14 -Wall -Wextra -Wpedantic

// Convert this program to C++
// change to C++ io
// change to one line comments
// change defines of constants to const
// change array to vector<>
// inline any short function

#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// the sum of all elements in vector `v` of a type `__t` defaulted to int
template <typename __t = int> inline __t sum(const vector<__t> &v) {
    // it's better to use STL's functions when possible
    // this sums the whole vector starting with 0
    return accumulate(begin(v), end(v), 0);
}

// returns a sample vector of type `__t` defaulted to int
template <typename __t = int> inline vector<__t> populate(int size) {
    vector<__t> v;

    // it's practical to reserve memory if the size is known before hand
    // instead of keep pushing while the vector changes the size accordingly
    v.reserve(size);
    for (int i = 0; i < size; ++i)
        v.push_back(i);
    return v;
}

int main(int, char *[]) {
    const int N = 40;
    const vector<int> data = populate(N);
    cout << "sum is " << sum(data) << endl;
    return 0;
}
