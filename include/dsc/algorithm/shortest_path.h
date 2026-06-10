#ifndef DSC_ALGORITHM_SHORTEST_PATH_H
#define DSC_ALGORITHM_SHORTEST_PATH_H

#include <vector>
#include <limits>
#include <algorithm>

namespace dsc {
namespace algorithm {

// ============================================================
// 最短路径算法
// 所有算法使用整数顶点索引 [0, V-1]
// INF 表示不可达
// ============================================================

const int INF = std::numeric_limits<int>::max() / 2;

// -------------------- Dijkstra 单源最短路径 --------------------
// 适用于非负权图
// graph: 邻接矩阵 graph[u][v] = 权重，graph[u][v] = INF 表示无边
// start: 源点
// 返回 dist 数组，dist[i] = start 到 i 的最短距离
inline std::vector<int> Dijkstra(
    const std::vector<std::vector<int>>& graph, int start) {

    int V = static_cast<int>(graph.size());
    std::vector<int> dist(V, INF);
    std::vector<bool> visited(V, false);
    dist[start] = 0;

    for (int i = 0; i < V; ++i) {
        // 选取未访问顶点中 dist 最小的
        int u = -1;
        int min_dist = INF;
        for (int j = 0; j < V; ++j) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = true;

        // 松弛
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && graph[u][v] != 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    return dist;
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
