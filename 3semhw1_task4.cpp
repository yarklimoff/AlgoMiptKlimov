// https://contest.yandex.ru/contest/20019/run-report/39565032/
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#define alphabet 123

void FirstStep(std::vector<int> &cnt, const std::string& s,
               std::vector<int> &class_id, std::vector<int> &suf_array,
               int &classes) {
    size_t n = s.length();
    for (int i = 0; i < n; ++i) {
        ++cnt[s[i]];
    }
    for (int i = 1; i < alphabet; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = 0; i < n; ++i) {
        suf_array[--cnt[s[i]]] = i;
    }
    for (int i = 1; i < n; ++i) {
        class_id[suf_array[i]] = class_id[suf_array[i - 1]];
        if (s[suf_array[i]] != s[suf_array[i - 1]]) {
            ++class_id[suf_array[i]];
            ++classes;
        }
    }
}

void OtherSteps(std::vector<int> &cnt, const std::string& s,
                std::vector<int> &class_id, std::vector<int> &suf_array,
                int &classes) {
    size_t n = s.length();
    std::vector<int> temp_suf_array(n, 0), temp_classes(n, 0);
    for (int len = 1; len < n; len *= 2) {
        for (int i = 0; i < n; ++i) {
            temp_suf_array[i] = suf_array[i] - len;
            if (temp_suf_array[i] < 0) {
                temp_suf_array[i] += n;
            }
        }
        cnt = std::vector<int>(classes, 0);
        for (int i = 0; i < n; ++i) {
            ++cnt[class_id[temp_suf_array[i]]];
        }
        for (int i = 1; i < classes; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (ssize_t i = n - 1; i >= 0; --i) {
            suf_array[--cnt[class_id[temp_suf_array[i]]]] = temp_suf_array[i];
        }
        temp_classes[temp_suf_array[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (suf_array[i] + len) % n,  mid2 = (suf_array[i - 1] + len) % n;
            if (class_id[suf_array[i]] != class_id[suf_array[i - 1]] ||
                class_id[mid1] != class_id[mid2]) {
                ++classes;
            }
            temp_classes[suf_array[i]] = classes - 1;
        }
        class_id = temp_classes;
    }
}

std::vector<int> BuildSuffArray(const std::string& s) {
    size_t n = s.length();
    int classes = 1;
    std::vector<int> cnt(alphabet, 0), suf_array(n, 0), class_id(n, 0);
    FirstStep(cnt, s, class_id, suf_array, classes);
    OtherSteps(cnt, s, class_id, suf_array, classes);
    return suf_array;
}



std::vector<int> BuildLCP(std::vector<int>& suf_array, const std::string& s) {
    size_t n = s.length();
    std::vector<int> rev_suf_array(n, 0);
    std::vector<int> LCP(n, 0);
    for (int i = 0; i < n; ++i) {
        rev_suf_array[suf_array[i]] = i;
    }
    int curLCP = 0;
    for (int i = 0; i < n; ++i) {
        int p = rev_suf_array[i];
        if (p == n - 1) {
            LCP[p] = 0;
            curLCP = 0;
            continue;
        }
        int q = suf_array[p + 1];
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

void Solve(std::string& s) {
    std::vector<int> suff_array, LCP;
    s += '\0';
    size_t n = s.length();
    suff_array = BuildSuffArray(s);
    LCP = BuildLCP(suff_array, s);
    long long ans = n * n - accumulate(suff_array.begin(), suff_array.end(), 0)
                    - std::accumulate(LCP.begin(), LCP.end(), 0) - n;
    std::cout << ans;
}

int main() {
    std::string s;
    std::cin >> s;
    Solve(s);
    return 0;
}
