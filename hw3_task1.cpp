#include "algorithm"
#include "iostream"
#include "utility"
#include "vector"

#define INF 1000000
enum Bool {False = 0, True};

struct Edge {
    long start = 0, end = 0, weight = INF;
};

void find_easiest_edge(std::vector<std::vector<std::pair<long, long>>>& graph,
        std::vector<Bool>& used, std::vector<long>& MST, Edge& curr_min_edge) {
    curr_min_edge.weight = INF;
    for (long i : MST) {
        for (auto j : graph[i]) {
            if (used[j.second] == False) {
                if (curr_min_edge.weight > j.first) {
                    curr_min_edge.weight = j.first;
                    curr_min_edge.start = i;
                    curr_min_edge.end = j.second;
                }
                break;
            }
        }
    }
    used[curr_min_edge.end] = True;
}


void build_MST(std::vector<std::vector<std::pair<long, long>>>& graph) {
    Edge curr_min_edge;
    std::vector<Bool> used(graph.size(), False);
    long min_weight = 0;
    std::vector<long> MST;
    MST.push_back(0);
    used[0] = True;
    for (long i = 0; i < graph.size() - 1; ++i) {
        find_easiest_edge(graph, used, MST, curr_min_edge);
        MST.push_back(curr_min_edge.end);
        min_weight += curr_min_edge.weight;
    }
    std::cout << min_weight;
}


int main() {
    long n, m, i, end, weight, start;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<long, long>>> graph(n, std::vector<std::pair<long, long>>());
    for (i = 0; i < m; ++i) {
        std::cin >> start >> end >> weight;
        --start, --end;
        graph[start].push_back(std::make_pair(weight, end));
        graph[end].push_back(std::make_pair(weight, start));
    }
    for (i = 0; i < n; ++i) {
        sort(graph[i].begin(), graph[i].end());
    }
    build_MST(graph);
    return 0;
}

