# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Test

```bash
# Configure (MinGW — the only preset)
cmake --preset mingw

# Build all test targets
cmake --build build

# Run all tests
ctest --test-dir build

# Run a single test by name
ctest --test-dir build -R test_vector

# Build and run a single test directly
cmake --build build --target test_vector && ./build/tests/test_vector
```

Tests are standalone executables using raw `assert()`. Each prints `"<name>: all tests passed!"` on success and aborts on first failure (no test framework).

## Architecture

This is a **header-only C++17 library** — all template implementations live in the headers alongside declarations. There is no `.cpp` compilation unit for the library itself; the CMake build exists only to compile and run tests.

### Source layout

```
include/dsc/
├── dsc.h                       # Umbrella include — use this to pull in everything
├── <data_structure>.h          # One header per data structure
└── algorithm/
    ├── algorithm.h             # Umbrella include for algorithms
    └── <algorithm>.h           # One header per algorithm category
```

### Key conventions

- **Namespace**: data structures are in `dsc::`; algorithms are in `dsc::algorithm::`
- **Naming**: classes `CamelCase`, functions `CamelCase` (e.g. `PushBack`, `QuickSort`), member variables `snake_case_` with trailing underscore, private helpers `snake_case_` with trailing underscore
- **Self-contained**: each header is independently readable — includes only what it needs and defines the full implementation
- **No iterators**: all access is via index (`operator[]`, `at()`) or raw pointer (`data()`)
- **No exceptions except** `std::out_of_range` on bounds-checked access methods like `at()`

### Adding a new data structure or algorithm

1. Create the header in `include/dsc/` (or `include/dsc/algorithm/` for algorithms)
2. Add `#include "new_file.h"` to the umbrella header (`dsc.h` or `algorithm/algorithm.h`)
3. Add a test file `tests/test_new_thing.cpp` using raw `assert()`
4. Register the test in `tests/CMakeLists.txt`: `add_dsc_test(test_new_thing)`

### Graph module notes

The `graph.h` header provides two implementations:
- `dsc::AdjListGraph` — adjacency list, supports directed/undirected
- `dsc::AdjMatrixGraph` — adjacency matrix, supports directed/undirected

Graph algorithms (Dijkstra, Floyd, Kruskal, Prim, topological sort, critical path) are separate headers under `algorithm/` and work with the graph classes.

### Test patterns

Tests follow a consistent structure:
```cpp
#include <dsc/vector.h>
#include <iostream>
#include <cassert>

int main() {
    // Exercise the component
    dsc::Vector<int> v;
    v.push_back(1);
    assert(v.size() == 1);
    assert(v[0] == 1);

    std::cout << "test_vector: all tests passed!" << std::endl;
    return 0;
}
```

`<cassert>` is always used — `assert()` calls abort on failure, so a non-zero exit from a test executable means a test failed.
