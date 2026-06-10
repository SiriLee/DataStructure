#include <dsc/algorithm/sort.h>
#include <iostream>
#include <vector>
#include <cassert>

bool is_sorted(const std::vector<int>& arr) {
    for (std::size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

int main() {
    using namespace dsc::algorithm;

    std::vector<int> original = {64, 34, 25, 12, 22, 11, 90, 45, 33, 77};

    // BubbleSort
    {
        auto arr = original;
        BubbleSort(arr);
        assert(is_sorted(arr));
    }

    // InsertionSort
    {
        auto arr = original;
        InsertionSort(arr);
        assert(is_sorted(arr));
    }

    // ShellSort
    {
        auto arr = original;
        ShellSort(arr);
        assert(is_sorted(arr));
    }

    // SelectionSort
    {
        auto arr = original;
        SelectionSort(arr);
        assert(is_sorted(arr));
    }

    // HeapSort
    {
        auto arr = original;
        HeapSort(arr);
        assert(is_sorted(arr));
    }

    // QuickSort (recursive)
    {
        auto arr = original;
        QuickSort(arr, 0, static_cast<int>(arr.size()) - 1);
        assert(is_sorted(arr));
    }

    // QuickSortIterative
    {
        auto arr = original;
        QuickSortIterative(arr);
        assert(is_sorted(arr));
    }

    // MergeSort (iterative)
    {
        auto arr = original;
        MergeSort(arr);
        assert(is_sorted(arr));
    }

    // MergeSortRecursive
    {
        auto arr = original;
        MergeSortRecursive(arr, 0, static_cast<int>(arr.size()));
        assert(is_sorted(arr));
    }

    // RadixSortLSD
    {
        std::vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66};
        RadixSortLSD(arr, 3);
        assert(is_sorted(arr));
    }

    // 边界情况：空数组
    {
        std::vector<int> arr;
        BubbleSort(arr);
        QuickSortIterative(arr);
        assert(arr.empty());
    }

    // 边界情况：单元素
    {
        std::vector<int> arr = {42};
        BubbleSort(arr);
        assert(arr.size() == 1 && arr[0] == 42);
    }

    std::cout << "test_sort: all tests passed!" << std::endl;
    return 0;
}
