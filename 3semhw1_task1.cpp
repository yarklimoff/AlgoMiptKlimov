// https://contest.yandex.ru/contest/19772/run-report/36816736/
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

std::vector<int> PrefixFunction (const std::string& s) {
    int n = (int) s.length();
    std::vector<int> pi (n, 0);
    for (size_t i = 1; i < n; ++i) {
        int k = pi[i - 1];
        while (k > 0 && s[i] != s[k])
            k = pi[k - 1];
        if (s[i] == s[k])
            ++k;
        pi[i] = k;
    }
    return pi;
}

void FindInclusion(const std::string& s, const std::string& pattern){
    std::vector<int> prefix_f_pattern = PrefixFunction(pattern);
    int last_prefix = 0;
    for (size_t i = 0; i < s.length(); ++i) {
        while (last_prefix > 0 && pattern[last_prefix] != s[i]){
            last_prefix = prefix_f_pattern[last_prefix - 1];
        }
     
        if (pattern[last_prefix] == s[i]) {
            ++last_prefix;
        }
     
        if (last_prefix == pattern.length()) {
            std::cout << i + 1 - pattern.length() << " ";
            }
        }
}


int main() {
    std::string s, pattern;
    std::cin >> pattern >> s;
    FindInclusion(s, pattern);
    return 0;
}
