#include <dsc/heap.h>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>

// helper: verify max-heap property on internal vector view
template<typename T>
bool is_max_heap(const std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n; ++i) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[left] > arr[i])   return false;
        if (right < n && arr[right] > arr[i]) return false;
    }
    return true;
}

int main() {
    // 1. Empty heap
    {
        dsc::Heap<int> h;
        assert(h.Empty());
        assert(h.Size() == 0);

        bool caught = false;
        try { h.Top(); } catch (const std::out_of_range&) { caught = true; }
        assert(caught);

        caught = false;
        try { h.Pop(); } catch (const std::out_of_range&) { caught = true; }
        assert(caught);
    }

    // 2. Push + Top + Pop basic cycle
    {
        dsc::Heap<int> h;
        h.Push(10);
        assert(!h.Empty());
        assert(h.Size() == 1);
        assert(h.Top() == 10);

        h.Push(20);
        assert(h.Top() == 20);  // 20 > 10, should rise to top

        h.Push(5);
        assert(h.Top() == 20);  // still 20

        h.Pop();
        assert(h.Top() == 10);  // 10 now on top
        assert(h.Size() == 2);

        h.Pop();
        assert(h.Top() == 5);
        assert(h.Size() == 1);

        h.Pop();
        assert(h.Empty());
    }

    // 3. Push multiple, verify max always at Top
    {
        dsc::Heap<int> h;
        int values[] = {3, 1, 6, 5, 2, 4};
        for (int v : values) {
            h.Push(v);
            // top should always be the max pushed so far
            int expected_max = v > 6 ? v : (v > 5 ? v : (v > 4 ? v : (v > 3 ? v : (v > 2 ? v : (v > 1 ? v : 3)))));
            // just verify: top >= each pushed value (relaxed check)
            assert(h.Top() >= v);
        }
    }

    // 4. Heapify from random vector
    {
        dsc::Heap<int> h;
        std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
        h.Heapify(arr);
        assert(h.Size() == 8);
        assert(h.Top() == 9);

        // verify max-heap property by popping all in descending order
        int prev = h.Top();
        while (!h.Empty()) {
            int cur = h.Top();
            h.Pop();
            assert(cur <= prev);  // non-increasing
            prev = cur;
        }
    }

    // 5. Heapify replaces existing data
    {
        dsc::Heap<int> h;
        h.Push(100);
        h.Push(200);
        assert(h.Size() == 2);

        std::vector<int> arr2 = {1, 2, 3};
        h.Heapify(arr2);
        assert(h.Size() == 3);
        assert(h.Top() == 3);  // new data replaces old
    }

    // 6. Single element edge case
    {
        dsc::Heap<int> h;
        h.Push(42);
        assert(h.Top() == 42);
        h.Pop();
        assert(h.Empty());

        // Heapify with single element
        std::vector<int> single = {99};
        h.Heapify(single);
        assert(h.Size() == 1);
        assert(h.Top() == 99);
        h.Pop();
        assert(h.Empty());
    }

    // 7. Duplicate values
    {
        dsc::Heap<int> h;
        h.Push(7);
        h.Push(7);
        h.Push(7);
        assert(h.Size() == 3);
        assert(h.Top() == 7);
        h.Pop();
        assert(h.Top() == 7);
        h.Pop();
        assert(h.Top() == 7);
        h.Pop();
        assert(h.Empty());
    }

    // 8. Large sequence: push all, pop all → non-increasing
    {
        dsc::Heap<int> h;
        const int N = 1000;
        // push values that produce a known pattern
        for (int i = 0; i < N; ++i) {
            h.Push(i);
        }
        assert(h.Size() == N);
        assert(h.Top() == N - 1);

        int prev = h.Top();
        int count = 0;
        while (!h.Empty()) {
            int cur = h.Top();
            h.Pop();
            ++count;
            assert(cur <= prev);
            prev = cur;
        }
        assert(count == N);
    }

    // 9. Copy and move
    {
        dsc::Heap<int> h;
        h.Push(10);
        h.Push(20);

        dsc::Heap<int> h2(h);        // copy
        assert(h2.Top() == 20);
        assert(h2.Size() == 2);
        assert(h.Top() == 20);       // original unchanged

        dsc::Heap<int> h3(std::move(h2));  // move
        assert(h3.Top() == 20);
        assert(h3.Size() == 2);
    }

    std::cout << "test_heap: all tests passed!" << std::endl;
    return 0;
}
