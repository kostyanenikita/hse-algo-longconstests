#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

const int V_MAX = 200, E_MAX = 8000, INF = std::numeric_limits<int>::max();
int n, m, k, cnt;
int lasts[V_MAX], edges[E_MAX], costs[E_MAX], prevs[E_MAX], flows[E_MAX], capacities[E_MAX];

void add_edge(int from, int to, int cost, int capacity) {
    edges[cnt] = to;
    costs[cnt] = cost;
    prevs[cnt] = lasts[from];
    capacities[cnt] += capacity;
    lasts[from] = cnt;
    ++cnt;
}

int d[V_MAX], p[V_MAX], e[V_MAX];

int phi[V_MAX];

void dijkstra() {
    for (int i = 0; i != n; ++i) {
        d[i] = std::numeric_limits<int>::max();
        p[i] = -1;
        e[i] = -1;
    }
    d[0] = 0;
    std::set<std::pair<int, int> > s;
    s.insert({d[0], 0});
    while (!s.empty()) {
        int from = s.begin()->second;
        s.erase(s.begin());
        for (int edge = lasts[from]; edge != -1; edge = prevs[edge]) {
            int to = edges[edge], cost = costs[edge] + phi[from] - phi[to];
            if (flows[edge] < capacities[edge] && d[to] > d[from] + cost) {
                s.erase({d[to], to});
                d[to] = d[from] + cost;
                p[to] = from;
                e[to] = edge;
                s.insert({d[to], to});
            }
        }
    }
}

void fill() {
    for (int to = n - 1; p[to] != -1; to = p[to]) {
        int edge = e[to];
        ++flows[edge];
        --flows[edge ^ 1];
    }
}

std::vector<int> path;
std::vector<std::vector<int> > paths;
int sum;
int used[V_MAX];

bool dfs(int from) {
    if (from == n - 1) {
        return true;
    }
    used[from] = true;
    for (int edge = lasts[from]; edge != -1; edge = prevs[edge]) {
        int to = edges[edge], cost = costs[edge];
        if (!used[to] && flows[edge] > 0 && dfs(to)) {
            --flows[edge];
            ++flows[edge ^ 1];
            path.push_back(edge / 4 + 1);
            sum += cost;
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin >> n >> m >> k;
    for (int i = 0; i != n; ++i) {
        lasts[i] = -1;
    }
    for (int i = 0; i != m; ++i) {
        int from, to, cost;
        std::cin >> from >> to >> cost;
        --from, --to;
        add_edge(from, to, cost, 1);
        add_edge(to, from, -cost, 0);
        add_edge(to, from, cost, 1);
        add_edge(from, to, -cost, 0);
    }
    for (int i = 0; i != k; ++i) {
        dijkstra();
        if (d[n - 1] == INF) {
            std::cout << -1 << '\n';
            return 0;
        }
        fill();
        for (int j = 0; j != n; ++j) {
            if (d[j] != INF) {
                phi[j] += d[j];
            }
        }
    }
    for (int i = 0; i != k; ++i) {
        path.clear();
        for (int j = 0; j != n; ++j) {
            used[j] = false;
        }
        dfs(0);
        std::reverse(path.begin(), path.end());
        paths.push_back(path);
    }
    std::cout.precision(20);
    std::cout << (long double)sum / k << '\n';
    for (int i = 0; i != paths.size(); ++i) {
        std::cout << paths[i].size() << ' ';
        for (int j = 0; j != paths[i].size(); ++j) {
            std::cout << paths[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
