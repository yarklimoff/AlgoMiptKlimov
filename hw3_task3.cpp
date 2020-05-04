#include "algorithm"
#include "iostream"
#include "vector"

struct Edge {
    long long start = 0, finish = 0, weight = 0;
    Edge() {}
    Edge(long long w, long long s, long long f) {
        start = s;
        finish = f;
        weight = w;
    }
};

bool cmp(Edge a, Edge b) {
    return a.weight < b.weight;
}

long long get_parent(long long x, std::vector<long long>& p) {
    if (x == p[x]) {
        return x;
    } else {
        return p[x] = get_parent(p[x], p);
    }
}

bool merge(long long x, long long y, std::vector<long long>& p) {
    x = get_parent(x, p);
    y = get_parent(y, p);
    if (x == y)
        return false;
    p[x] = y;
    return true;
}

void find_MST(long long& n, std::vector<long long>& a, std::vector<Edge>& special) {
    long long ind_min_elem = 0;
    std::vector<long long> parent(n);
    for (long long i = 1; i < n; ++i) {
        if (a[ind_min_elem] > a[i]) {
            ind_min_elem = i;
        }
    }
    for (int i = 0; i < n; ++i)
        if (i != ind_min_elem)
            special.push_back(Edge(a[i] + a[ind_min_elem], i, ind_min_elem));
    sort(special.begin(), special.end(), cmp);
    long long ans = 0;
    for (int i = 0; i < n; ++i)
        parent[i] = i;
    for (auto e : special)
        if (merge(e.start, e.finish, parent))
            ans += e.weight;
    std::cout << ans;
}

int main() {
    long long n, m;
    std::cin >> n >> m;
    std::vector<long long> a(n);
    for (long long i = 0; i < n; i++)
        std::cin >> a[i];
    std::vector<Edge> special(m);
    for (int i = 0; i < m; i++) {
        std::cin >> special[i].start >> special[i].finish >> special[i].weight;
        --special[i].start;
        --special[i].finish;
    }
    find_MST(n, a, special);
    return 0;
}

