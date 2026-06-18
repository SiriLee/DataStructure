#include <dsc/algorithm/shortest_path.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    int INF = 0x3f3f3f3f;

    // Dijkstra
    {
        std::vector<std::vector<int>> graph = {
            {0, 4, 0, 0, 0, 0},
            {4, 0, 8, 0, 0, 0},
            {0, 8, 0, 7, 0, 4},
            {0, 0, 7, 0, 9, 14},
            {0, 0, 0, 9, 0, 10},
            {0, 0, 4, 14, 10, 0},
        };
        // 将 0 边替换为 INF（除了对角线）
        for (auto& row : graph) {
            for (int& w : row) {
                if (w == 0) w = INF;
            }
        }
        for (int i = 0; i < 6; ++i) graph[i][i] = 0;

        auto dist = Dijkstra(graph, 0);
        // 0->1 = 4, 0->2 = 12, 0->3 = 19, 0->4 = 26 (via 2->3->4), 0->5 = 16
        assert(dist[0] == 0);
        assert(dist[1] == 4);

        // DijkstraNaive should produce the same result
        auto dist_naive = DijkstraNaive(graph, 0);
        assert(dist_naive == dist);
    }

    // Floyd
    {
        std::vector<std::vector<int>> graph = {
            {0, 3, INF},
            {3, 0, 4},
            {INF, 4, 0},
        };
        Floyd(graph);
        assert(graph[0][2] == 7);  // 0->1->2 = 3+4 = 7
    }

    // Warshall
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 0},
        };
        Warshall(graph);
        assert(graph[0][2] == 1);  // 0 可达 2
    }

    std::cout << "test_shortest_path: all tests passed!" << std::endl;
    return 0;
}
