#ifndef DSC_ALGORITHM_SHORTEST_PATH_H
#define DSC_ALGORITHM_SHORTEST_PATH_H

#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <utility>

namespace dsc {
namespace algorithm {

// ============================================================
// 最短路径算法
// 所有算法使用整数顶点索引 [0, V-1]
// INF 表示不可达
// ============================================================

const int INF = std::numeric_limits<int>::max() / 2;

// -------------------- Dijkstra 单源最短路径（优先队列优化）--------------------
// 适用于非负权图
// graph: 邻接矩阵 graph[u][v] = 权重，graph[u][v] = INF 表示无边
// start: 源点
// 返回 (distances, previous vertices)
// 时间复杂度 O(E log V)，使用惰性删除避免 decrease_key
inline std::pair<std::vector<int>, std::vector<int>>
Dijkstra (const std::vector<std::vector<int>>& graph, int start) {
    int n = static_cast<int>(graph.size());

    std::vector<int> dist(n, INF); dist[start] = 0;
    std::vector<int> prev(n, -1);
    using Entry = std::pair<int, int>; // (distance, vertex)
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    pq.push({0, start});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int d = top.first;
        int u = top.second;
        if (d > dist[u]) continue; // 惰性删除：跳过过期条目
        for (int v = 0; v < n; ++v) {
            int weight = graph[u][v];
            if (weight != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, prev};
}

// -------------------- Floyd 全源最短路径 --------------------
// graph: 邻接矩阵 graph[u][v] = 权重，graph[u][v] = INF 表示无边
// 直接在 graph 上修改，返回所有顶点对的最短距离
inline void Floyd(std::vector<std::vector<int>>& graph) {
    int V = static_cast<int>(graph.size());
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (graph[i][k] != INF && graph[k][j] != INF &&
                    graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }
}

// -------------------- Warshall 传递闭包 --------------------
// graph: 邻接矩阵，graph[u][v] = 1 表示有边，0 表示无边
// 直接在 graph 上修改，graph[u][v] = 1 表示 u 可达 v
inline void Warshall(std::vector<std::vector<int>>& graph) {
    int V = static_cast<int>(graph.size());
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                graph[i][j] = graph[i][j] || (graph[i][k] && graph[k][j]);
            }
        }
    }
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_SHORTEST_PATH_H
