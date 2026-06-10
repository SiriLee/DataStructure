#ifndef DSC_ALGORITHM_SORT_H
#define DSC_ALGORITHM_SORT_H

#include <vector>
#include <utility>
#include <algorithm>
#include <stack>
#include <list>
#include <cassert>

namespace dsc {
namespace algorithm {

// ============================================================
// 排序算法集合
// 所有排序函数对 std::vector<T>& 进行原地排序，使用整数索引
// ============================================================

// -------------------- 冒泡排序 --------------------
template<typename T>
void BubbleSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // 已有序，提前退出
    }
}

// -------------------- 插入排序 --------------------
template<typename T>
void InsertionSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// -------------------- 希尔排序 --------------------
template<typename T>
void ShellSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            T key = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > key) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = key;
        }
    }
}

// -------------------- 选择排序 --------------------
template<typename T>
void SelectionSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
    }
}

// -------------------- 堆排序（手动建堆） --------------------
template<typename T>
void Heapify(std::vector<T>& arr, int n, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])   largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        Heapify(arr, n, largest);
    }
}

template<typename T>
void HeapSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());

    // 建堆
    for (int i = n / 2 - 1; i >= 0; --i) {
        Heapify(arr, n, i);
    }

    // 逐个提取堆顶元素
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        Heapify(arr, i, 0);
    }
}

// -------------------- 快速排序（递归版）--------------------
template<typename T>
int Partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template<typename T>
void QuickSort(std::vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = Partition(arr, low, high);
        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}

// -------------------- 快速排序（迭代版）--------------------
// 使用显式栈替代递归
template<typename T>
void QuickSortIterative(std::vector<T>& arr) {
    if (arr.empty()) return;
    int low  = 0;
    int high = static_cast<int>(arr.size()) - 1;

    std::stack<std::pair<int, int>> stk;
    stk.push({low, high});

    while (!stk.empty()) {
        auto [l, h] = stk.top();
        stk.pop();

        if (l >= h) continue;

        int pi = Partition(arr, l, h);
        stk.push({l, pi - 1});
        stk.push({pi + 1, h});
    }
}

// -------------------- 归并排序辅助：合并两个有序段 --------------------
template<typename T>
void Merge(std::vector<T>& arr, int left, int mid, int right) {
    // [left, mid) 和 [mid, right) 是两个有序段
    std::vector<T> temp;
    temp.reserve(right - left);
    int i = left, j = mid;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }
    while (i < mid)  temp.push_back(arr[i++]);
    while (j < right) temp.push_back(arr[j++]);

    for (std::size_t k = 0; k < temp.size(); ++k) {
        arr[left + k] = temp[k];
    }
}

// -------------------- 归并排序（迭代版，自底向上）--------------------
template<typename T>
void MergeSort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = std::min(left + size, n);
            int right = std::min(left + 2 * size, n);
            Merge(arr, left, mid, right);
        }
    }
}

// -------------------- 归并排序（递归版，自顶向下）--------------------
template<typename T>
void MergeSortRecursive(std::vector<T>& arr, int left, int right) {
    // [left, right) 半开区间
    if (left < right - 1) {
        int mid = left + (right - left) / 2;
        MergeSortRecursive(arr, left, mid);
        MergeSortRecursive(arr, mid, right);
        Merge(arr, left, mid, right);
    }
}

// -------------------- 基数排序（LSD，仅适用于非负整数） --------------------
template<typename T>
T DigitAt(const T& num, int d) {
    T ten = static_cast<T>(10);
    T val = num;
    for (int i = 0; i < d; ++i) {
        val /= ten;
    }
    return val % ten;
}

template<typename T>
void RadixSortLSD(std::vector<T>& arr, int max_digits) {
    // 要求所有元素非负
    for (const T& num : arr) {
        (void)num;
        assert(num >= 0);
    }

    std::vector<std::list<T>> buckets(10);
    for (int d = 0; d < max_digits; ++d) {
        for (const T& num : arr) {
            int digit = static_cast<int>(DigitAt(num, d));
            buckets[digit].push_back(num);
        }
        int idx = 0;
        for (auto& bucket : buckets) {
            for (const T& num : bucket) {
                arr[idx++] = num;
            }
            bucket.clear();
        }
    }
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_SORT_H
