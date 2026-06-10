#include <dsc/graph.h>
#include <iostream>
#include <vector>
#include <cassert>

// 用于收集遍历结果的辅助
std::vector<int> visited_order;
void collect(int v) { visited_order.push_back(v); }

int main() {
    // ---- AdjListGraph 无向图 ----
    {
        dsc::AdjListGraph<false> g(5);
        g.add_edge(0, 1);
        g.add_edge(0, 2);
        g.add_edge(1, 3);
        g.add_edge(2, 4);

        assert(g.vertices() == 5);

        // BFS
        visited_order.clear();
        g.bfs(0, collect);
        assert(visited_order.size() == 5);

        // DFS
        visited_order.clear();
        g.dfs(0, collect);
        assert(visited_order.size() == 5);

        // 连通性
        assert(g.is_connected());
    }

    // ---- AdjListGraph 有向图 ----
    {
        dsc::AdjListGraph<true> g(4);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_edge(0, 3);

        visited_order.clear();
        g.bfs(0, collect);
        assert(visited_order.size() == 4);
    }

    // ---- AdjMatrixGraph 无向图 ----
    {
        dsc::AdjMatrixGraph<false> g(4);
        g.add_edge(0, 1, 5);
        g.add_edge(1, 2, 3);
        g.add_edge(2, 3, 1);

        assert(g.get_weight(0, 1) == 5);
        assert(g.get_weight(1, 0) == 5);  // 无向：对称
        assert(g.has_edge(0, 1));
        assert(!g.has_edge(0, 2));

        visited_order.clear();
        g.bfs(0, collect);
        assert(visited_order.size() == 4);
    }

    // ---- AdjMatrixGraph 有向图 ----
    {
        dsc::AdjMatrixGraph<true> g(3);
        g.add_edge(0, 1, 10);
        g.add_edge(1, 2, 20);

        assert(g.get_weight(0, 1) == 10);
        assert(g.get_weight(1, 0) == dsc::AdjMatrixGraph<true>::INF);  // 有向：不对称

        visited_order.clear();
        g.dfs(0, collect);
        assert(visited_order.size() == 3);
    }

    std::cout << "test_graph: all tests passed!" << std::endl;
    return 0;
}
