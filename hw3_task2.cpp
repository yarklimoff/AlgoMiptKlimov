#include "algorithm"
#include "iostream"
#include "vector"

struct Edge {
    long start = 0, end = 0, weight = 0;
    Edge(long w, long s, long e){
        start = s;
        end = e;
        weight = w;
    }
};

bool cmp(Edge& a, Edge& b){
    return a.weight < b.weight;
}

long get(long& v, std::vector<long>& parent){
    if (parent[v] == v)
        return v;
    return parent[v] = get(parent[v], parent);
}

void unite(long& u, long& v, std::vector<long>& parent, std::vector<long>& rank){
    u = get(u, parent);
    v = get(v, parent);
    if (u != v) {
        if(rank[u] < rank[v])
            std::swap(u,v);
        parent[v] = u;
        if(rank[u]==rank[v]){
            ++rank[u];
        }
    }
}

void build_MST(std::vector<Edge>& graph, long& n) {
    long min_weight = 0;
    std::vector<long> parent(n);
    std::vector<long> rank(n, 0);
    for (long i = 0; i < n; ++i) {
        parent[i] = i;
    }
    for (long i = 0; i < graph.size() ; ++i) {
        long a = graph[i].start, b = graph[i].end, l = graph[i].weight;
        if (get(a, parent) != get(b, parent)) {
            min_weight += l;
            unite(a, b, parent, rank);
        }
    }
    std::cout << min_weight;
}


int main() {
    long n, m, i, end, weight, start;
    std::cin >> n >> m;
    std::vector<Edge> graph;
    for (i = 0; i < m; ++i) {
        std::cin >> start >> end >> weight;
        --start, --end;
        graph.push_back(Edge(weight, start, end));
    }
    sort(graph.begin(), graph.end(), cmp);
    build_MST(graph, n);
    return 0;
}

