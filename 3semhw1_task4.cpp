// https://contest.yandex.ru/contest/20019/run-report/36993813/
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#define alphabet 27

std::vector<int> BuildSuffMass(const std::string& s) {
    size_t n = s.length();
    int classes = 1;
    std::vector<int> cnt(alphabet, 0), suf_mass(n, 0), class_id(n, 0);
    std::vector<int> temp_suf_mass(n, 0), temp_classes(n, 0);
    for (int i = 0; i < n; ++i) {
        ++cnt[s[i]];
    }
    for (int i = 1; i < alphabet; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = 0; i < n; ++i) {
        suf_mass[--cnt[s[i]]] = i;
    }
    for (int i = 1; i < n; ++i) {
        class_id[suf_mass[i]] = class_id[suf_mass[i - 1]];
        if (s[suf_mass[i]] != s[suf_mass[i - 1]]) {
            ++class_id[suf_mass[i]];
            ++classes;
        }
    }
    for (int len = 1; len < n; len *= 2) {
        for (int i = 0; i < n; ++i) {
            temp_suf_mass[i] = suf_mass[i] - len;
            if (temp_suf_mass[i] < 0) {
                temp_suf_mass[i] += n;
            }
        }
        cnt = std::vector<int>(classes, 0);
        for (int i = 0; i < n; ++i) {
            ++cnt[class_id[temp_suf_mass[i]]];
        }
        for (int i = 1; i < classes; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (ssize_t i = n - 1; i >= 0; --i) {
            suf_mass[--cnt[class_id[temp_suf_mass[i]]]] = temp_suf_mass[i];
        }
        temp_classes[temp_suf_mass[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (suf_mass[i] + len) % n,  mid2 = (suf_mass[i - 1] + len) % n;
            if (class_id[suf_mass[i]] != class_id[suf_mass[i-1]] || class_id[mid1] != class_id[mid2]) {
                ++classes;
            }
            temp_classes[suf_mass[i]] = classes - 1;
        }
        class_id = temp_classes;
    }
    return suf_mass;
}

std::vector<int> buildLCP(std::vector<int>& suf_mass, const std::string& s) {
    size_t n = s.length();
    std::vector<int> rev_suf_mass(n, 0);
    std::vector<int> LCP(n, 0);
    for (int i = 0; i < n; ++i) {
        rev_suf_mass[suf_mass[i]] = i;
    }
    int curLCP = 0;
    for (int i = 0; i < n; ++i) {
        int p = rev_suf_mass[i];
        if (p == n - 1) {
            LCP[p] = 0;
            curLCP = 0;
            continue;
        }
        int q = suf_mass[p + 1];
        --curLCP;
        if (curLCP < 0) {
            curLCP = 0;
        }
        while (i + curLCP < n && q + curLCP < n && s[i + curLCP] == s[q + curLCP]) {
            ++curLCP;
        }
        LCP[p] = curLCP;
    }
    return LCP;
}

int main() {
    std::string s;
    std::cin >> s;
    std::vector<int> suff_mass, LCP;
    s += '`'; // ascii: '`' - 96, 'a' - 97
    for (int i = 0; i < s.length(); ++i) {
        s[i] -= '`';
    }
    size_t n = s.length();
    suff_mass = BuildSuffMass(s);
    LCP = buildLCP(suff_mass, s);
    long long ans = accumulate(suff_mass.begin(), suff_mass.end(), 0);
    ans = n * n - ans;
    ans -= std::accumulate(LCP.begin(), LCP.end(), 0);
    std::cout << ans - n;
    return 0;
}
