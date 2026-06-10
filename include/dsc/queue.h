#ifndef DSC_QUEUE_H
#define DSC_QUEUE_H

#include <cstddef>
#include <utility>

namespace dsc {

// ============================================================
// Queue<T> — 队列（FIFO）
// 基于环形缓冲区实现，支持自动扩容
// ============================================================
template<typename T>
class Queue {
public:
    // 构造 / 析构
    Queue() = default;
    explicit Queue(std::size_t capacity);
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    ~Queue();

    // 赋值
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;

    // 容量
    bool        empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);

    // 元素访问
    T&       front();
    const T& front() const;
    T&       back();
    const T& back() const;

    // 修改
    void push(const T& value);
    void push(T&& value);
    void pop();
    void clear() noexcept;
    void swap(Queue& other) noexcept;

private:
    T*          data_     = nullptr;
    std::size_t capacity_ = 0;
    std::size_t head_     = 0;  // 队首元素索引
    std::size_t tail_     = 0;  // 下一个元素插入位置
    std::size_t size_     = 0;

    void reallocate_(std::size_t new_cap);
};

// ==================== 构造 / 析构 ====================

template<typename T>
Queue<T>::Queue(std::size_t capacity)
    : data_(new T[capacity]), capacity_(capacity), head_(0), tail_(0), size_(0) {}

template<typename T>
Queue<T>::Queue(const Queue& other)
    : data_(new T[other.capacity_]), capacity_(other.capacity_),
      head_(0), tail_(other.size_), size_(other.size_) {
    // 将环形缓冲区中的元素按顺序复制到新数组头部
    for (std::size_t i = 0; i < size_; ++i) {
        std::size_t src_idx = (other.head_ + i) % other.capacity_;
        data_[i] = other.data_[src_idx];
    }
}

template<typename T>
Queue<T>::Queue(Queue&& other) noexcept
    : data_(other.data_), capacity_(other.capacity_),
      head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.data_     = nullptr;
    other.capacity_ = 0;
    other.head_     = 0;
    other.tail_     = 0;
    other.size_     = 0;
}

template<typename T>
Queue<T>::~Queue() {
    delete[] data_;
}

// ==================== 赋值 ====================

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        Queue temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_     = other.data_;
        capacity_ = other.capacity_;
        head_     = other.head_;
        tail_     = other.tail_;
        size_     = other.size_;
        other.data_     = nullptr;
        other.capacity_ = 0;
        other.head_     = 0;
        other.tail_     = 0;
        other.size_     = 0;
    }
    return *this;
}

// ==================== 容量 ====================

template<typename T>
bool Queue<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
std::size_t Queue<T>::size() const noexcept {
    return size_;
}

template<typename T>
std::size_t Queue<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
void Queue<T>::reserve(std::size_t new_cap) {
    if (new_cap > capacity_) {
        reallocate_(new_cap);
    }
}

// ==================== 元素访问 ====================

template<typename T>
T& Queue<T>::front() {
    return data_[head_];
}

template<typename T>
const T& Queue<T>::front() const {
    return data_[head_];
}

template<typename T>
T& Queue<T>::back() {
    std::size_t idx = (tail_ == 0) ? capacity_ - 1 : tail_ - 1;
    return data_[idx];
}

template<typename T>
const T& Queue<T>::back() const {
    std::size_t idx = (tail_ == 0) ? capacity_ - 1 : tail_ - 1;
    return data_[idx];
}

// ==================== 修改 ====================

template<typename T>
void Queue<T>::push(const T& value) {
    if (size_ >= capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[tail_] = value;
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
}

template<typename T>
void Queue<T>::push(T&& value) {
    if (size_ >= capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[tail_] = std::move(value);
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
}

template<typename T>
void Queue<T>::pop() {
    if (size_ > 0) {
        head_ = (head_ + 1) % capacity_;
        --size_;
    }
}

template<typename T>
void Queue<T>::clear() noexcept {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
}

template<typename T>
void Queue<T>::swap(Queue& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(capacity_, other.capacity_);
    swap(head_, other.head_);
    swap(tail_, other.tail_);
    swap(size_, other.size_);
}

// ==================== 私有辅助 ====================

template<typename T>
void Queue<T>::reallocate_(std::size_t new_cap) {
    T* new_data = new T[new_cap];
    // 将环形缓冲区中的元素按顺序复制到新数组头部
    for (std::size_t i = 0; i < size_; ++i) {
        std::size_t src_idx = (head_ + i) % capacity_;
        new_data[i] = data_[src_idx];
    }
    delete[] data_;
    data_     = new_data;
    capacity_ = new_cap;
    head_     = 0;
    tail_     = size_;  // 所有元素现在连续存放在 [0, size_)
}

} // namespace dsc

#endif // DSC_QUEUE_H
