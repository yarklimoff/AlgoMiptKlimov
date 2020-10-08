// https://contest.yandex.ru/contest/19772/run-report/35141594/
#include <iostream>
#include <vector>
#include <algorithm>
#include "string"

std::vector<int> prefix_function (const std::string& s) {
    int n = (int) s.length();
    std::vector<int> pi (n, 0);
    for (int i = 1; i < n; ++i) {
        int k = pi[i - 1];
        while (k > 0 && s[i] != s[k])
            k = pi[k - 1];
        if (s[i] == s[k])
            ++k;
        pi[i] = k;
    }
    return pi;
}



int main() {
    std::string s, pattern;
    std::cin >> pattern >> s;
    s = pattern + "$" + s;
    std::vector<int> result = prefix_function(s);
    for (size_t i = 0; i < s.length(); ++i) {
        if (result[i] == pattern.length())
            std::cout << i - 2 * pattern.length() << " ";
    }
    return 0;
}
