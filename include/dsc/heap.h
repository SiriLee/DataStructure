#ifndef DSC_HEAP_H
#define DSC_HEAP_H

#include <vector>
#include <utility>
#include <stdexcept>

namespace dsc {

// ============================================================
// Max-heap (大顶堆) — 0-based internal array
// ============================================================
template<typename T>
class Heap {
public:
    Heap() = default;

    void Push(const T& value);
    void Pop();
    const T& Top() const;
    void Heapify(std::vector<T> arr);
    int  Size() const;
    bool Empty() const;

private:
    void sift_up_(int i);
    void sift_down_(int i, int n);

    std::vector<T> data_;
};

// ======== Implementation ========

template<typename T>
void Heap<T>::Push(const T& value) {
    data_.push_back(value);
    sift_up_(static_cast<int>(data_.size()) - 1);
}

template<typename T>
void Heap<T>::Pop() {
    if (data_.empty()) {
        throw std::out_of_range("Heap::Pop: heap is empty");
    }
    data_[0] = std::move(data_.back());
    data_.pop_back();
    if (!data_.empty()) {
        sift_down_(0, static_cast<int>(data_.size()));
    }
}

template<typename T>
const T& Heap<T>::Top() const {
    if (data_.empty()) {
        throw std::out_of_range("Heap::Top: heap is empty");
    }
    return data_[0];
}

template<typename T>
void Heap<T>::Heapify(std::vector<T> arr) {
    data_ = std::move(arr);
    int n = static_cast<int>(data_.size());
    for (int i = n / 2 - 1; i >= 0; --i) {
        sift_down_(i, n);
    }
}

template<typename T>
int Heap<T>::Size() const {
    return static_cast<int>(data_.size());
}

template<typename T>
bool Heap<T>::Empty() const {
    return data_.empty();
}

template<typename T>
void Heap<T>::sift_up_(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!(data_[i] > data_[parent])) break;
        std::swap(data_[i], data_[parent]);
        i = parent;
    }
}

template<typename T>
void Heap<T>::sift_down_(int i, int n) {
    while (true) {
        int largest = i;
        int left    = 2 * i + 1;
        int right   = 2 * i + 2;
        if (left < n && data_[left] > data_[largest])   largest = left;
        if (right < n && data_[right] > data_[largest]) largest = right;
        if (largest == i) break;
        std::swap(data_[i], data_[largest]);
        i = largest;
    }
}

} // namespace dsc

#endif // DSC_HEAP_H
