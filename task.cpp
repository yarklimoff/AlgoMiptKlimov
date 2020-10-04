#include <iostream>
#include "algorithm"
#include "vector"
#include "string"

typedef long long ll;

ll CountingPalindromesOfEvenLength(size_t n, std::string& s) {
    std::vector<ll> odd(n, 0);
    ll left_bound = 0, right_bound = 0, ans = 0;
    for (ll i = 0; i < n; i++) {
        if (i < right_bound) {
            odd[i] = std::min(right_bound - i + 1,
                              odd[left_bound + right_bound - i + 1]);
        }
        while (i - odd[i] - 1 >= 0 && i + odd[i] < n &&
               s[i - odd[i] - 1] == s[i + odd[i]]) {
            ++odd[i];
        }
        if (i + odd[i] - 1 > right_bound) {
            left_bound = i - odd[i];
            right_bound = i + odd[i] - 1;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        ans += odd[i];
    }
    return ans;
}

ll CountingPalindromesOfOddLength(size_t n, std::string& s) {
    std::vector<ll> even(n, 0);
    ll left_bound = 0, right_bound = 0, ans = 0;
    for (ll i = 0; i < n; ++i) {
        if (i < right_bound) {
            even[i] = std::min(right_bound - i,
                               even[left_bound + right_bound - i]);
        }
        while (i - even[i] - 1 >= 0 && i + even[i] + 1 < n &&
               s[i - even[i] - 1] == s[i + even[i] + 1]) {
            even[i]++;
        }
        if (i + even[i] > right_bound) {
            left_bound = i - even[i];
            right_bound = i + even[i];
        }
    }
    for (size_t i = 0; i < n; ++i) {
        ans += even[i];
    }
    return ans;
}

int main() {
    std::string s;
    std::cin >> s;
    std::cout << CountingPalindromesOfOddLength(s.size(), s) +
                CountingPalindromesOfEvenLength(s.size(), s);
    return 0;
}
