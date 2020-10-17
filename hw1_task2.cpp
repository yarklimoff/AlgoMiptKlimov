// https://contest.yandex.ru/contest/19772/run-report/36813481/
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

long long CountingPalindromes(int a, const std::string& s) {
    size_t n = s.length();
    std::vector<int> counts_polindromes(n, 0);
    int left_bound = 0, right_bound = 0;
    for (int i = 0; i < n; ++i) {
        if (i < right_bound) {
            counts_polindromes[i] = std::min(right_bound - i + a,
                                             counts_polindromes[left_bound + right_bound - i + a]);
        }
        while (i - counts_polindromes[i] - 1 >= 0 && i + counts_polindromes[i] + (a^1)< n &&
               s[i - counts_polindromes[i] - 1] == s[i + counts_polindromes[i] + (a^1)]) {
            ++counts_polindromes[i];
        }
        if (i + counts_polindromes[i] - a > right_bound) {
            left_bound = i - counts_polindromes[i];
            right_bound = i + counts_polindromes[i] - a;
        }
    }
    long long ans = std::accumulate(counts_polindromes.begin(),
                                    counts_polindromes.end(), 0LL);
    return ans;
}


int main() {
    std::string s;
    std::cin >> s;
    std::cout << CountingPalindromes(0, s) +
                CountingPalindromes(1, s);
    return 0;
}
