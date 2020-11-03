#include <iostream>

int main()
{
    const int n = 5;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            std::cout << i << " " << j << "\n";
        }
    }
    std::cout << "---\n";
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i == j)
                continue;
            std::cout << i << " " << j << "\n";
        }
    }

    return 0;
}
