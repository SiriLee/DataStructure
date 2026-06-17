#include <dsc/algorithm/euler.h>
#include <iostream>
#include <cassert>

int main() {
    // ==================== Undirected ====================

    // Triangle (Euler circuit exists)
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1},
            {1, 0, 1},
            {1, 1, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(e.HasEulerCircuit());
        assert(e.HasEulerPath());

        auto circuit = e.EulerCircuit();
        assert(!circuit.empty());
        assert(circuit.front() == circuit.back());
    }

    // Path graph (Euler path exists, no circuit)
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(!e.HasEulerCircuit());
        assert(e.HasEulerPath());
        assert(e.EulerCircuit().empty());
    }

    // Star graph (4 odd-degree vertices — no Euler path or circuit)
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1, 1},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(!e.HasEulerCircuit());
        assert(!e.HasEulerPath());
    }

    // Disconnected graph
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 1},
            {0, 0, 1, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(!e.HasEulerCircuit());
        assert(!e.HasEulerPath());
    }

    // Graph with isolated vertex but connected component has circuit
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1, 0},
            {1, 0, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},  // isolated vertex 3
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(e.HasEulerCircuit());
        assert(e.HasEulerPath());
        auto circuit = e.EulerCircuit();
        assert(!circuit.empty());
        // start=-1 should select non-isolated vertex
        assert(e.EulerCircuit(-1) == circuit);
    }

    // Multiple edges (multigraph with Euler circuit)
    {
        std::vector<std::vector<int>> graph = {
            {0, 2, 0},
            {2, 0, 2},
            {0, 2, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(e.HasEulerCircuit());
        auto circuit = e.EulerCircuit();
        assert(!circuit.empty());
    }

    // ==================== Directed ====================

    // Directed cycle (Euler circuit exists)
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0},
            {0, 0, 1},
            {1, 0, 0},
        };
        dsc::algorithm::Eulerian e(graph, true);
        assert(e.HasEulerCircuit());
        assert(e.HasEulerPath());

        auto circuit = e.EulerCircuit();
        assert(!circuit.empty());
        assert(circuit.front() == circuit.back());
    }

    // Directed path (Euler path exists, no circuit)
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 0},
        };
        dsc::algorithm::Eulerian e(graph, true);
        assert(!e.HasEulerCircuit());
        assert(e.HasEulerPath());
    }

    // Directed graph with |out-in| > 1 — no circuit, no path
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
        };
        dsc::algorithm::Eulerian e(graph, true);
        assert(!e.HasEulerCircuit());
        assert(!e.HasEulerPath());
    }

    // ==================== Edge Cases ====================

    // Empty graph
    {
        std::vector<std::vector<int>> graph;
        dsc::algorithm::Eulerian e(graph, false);
        assert(!e.HasEulerCircuit());
        assert(!e.HasEulerPath());
        assert(e.EulerCircuit().empty());
    }

    // Single vertex, no edges
    {
        std::vector<std::vector<int>> graph = {{0}};
        dsc::algorithm::Eulerian e(graph, false);
        assert(!e.HasEulerCircuit());
        assert(!e.HasEulerPath());
        assert(e.EulerCircuit().empty());
    }

    // EulerCircuit with specified start vertex
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1},
            {1, 0, 1},
            {1, 1, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);

        auto circuit = e.EulerCircuit(1);
        assert(!circuit.empty());
        assert(circuit.front() == 1);
        assert(circuit.back() == 1);
    }

    // EulerCircuit with isolated start vertex
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1, 0},
            {1, 0, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
        };
        dsc::algorithm::Eulerian e(graph, false);
        assert(e.EulerCircuit(3).empty());
    }

    // Self-loop (undirected) — single vertex with one self-loop
    {
        std::vector<std::vector<int>> graph = {{1}};
        dsc::algorithm::Eulerian e(graph, false);
        assert(e.HasEulerCircuit());
        auto circuit = e.EulerCircuit();
        assert(!circuit.empty());
        assert(circuit.front() == 0 && circuit.back() == 0);
        // Circuit should be {0, 0} — start and end at 0 via self-loop
    }

    std::cout << "test_euler: all tests passed!" << std::endl;
    return 0;
}
