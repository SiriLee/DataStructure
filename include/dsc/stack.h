#ifndef DSC_STACK_H
#define DSC_STACK_H

#include <cstddef>
#include <utility>

namespace dsc {

// ============================================================
// Stack<T> — 栈（LIFO）
// 基于动态数组实现，支持自动扩容
// ============================================================
template<typename T>
class Stack {
public:
    // 构造 / 析构
    Stack() = default;
    explicit Stack(std::size_t capacity);
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    ~Stack();

    // 赋值
    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other) noexcept;

    // 容量
    bool        empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);

    // 元素访问
    T&       top();
    const T& top() const;

    // 修改
    void push(const T& value);
    void push(T&& value);
    void pop();
    void clear() noexcept;
    void swap(Stack& other) noexcept;

private:
    T*          data_     = nullptr;
    std::size_t size_     = 0;
    std::size_t capacity_ = 0;

    void reallocate_(std::size_t new_cap);
};

// ==================== 构造 / 析构 ====================

template<typename T>
Stack<T>::Stack(std::size_t capacity)
    : data_(new T[capacity]), size_(0), capacity_(capacity) {}

template<typename T>
Stack<T>::Stack(const Stack& other)
    : data_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template<typename T>
Stack<T>::Stack(Stack&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_     = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
}

template<typename T>
Stack<T>::~Stack() {
    delete[] data_;
}

// ==================== 赋值 ====================

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {
        Stack temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_     = other.data_;
        size_     = other.size_;
        capacity_ = other.capacity_;
        other.data_     = nullptr;
        other.size_     = 0;
        other.capacity_ = 0;
    }
    return *this;
}

// ==================== 容量 ====================

template<typename T>
bool Stack<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
std::size_t Stack<T>::size() const noexcept {
    return size_;
}

template<typename T>
std::size_t Stack<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
void Stack<T>::reserve(std::size_t new_cap) {
    if (new_cap > capacity_) {
        reallocate_(new_cap);
    }
}

// ==================== 元素访问 ====================

template<typename T>
T& Stack<T>::top() {
    return data_[size_ - 1];
}

template<typename T>
const T& Stack<T>::top() const {
    return data_[size_ - 1];
}

// ==================== 修改 ====================

template<typename T>
void Stack<T>::push(const T& value) {
    if (size_ >= capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[size_++] = value;
}

template<typename T>
void Stack<T>::push(T&& value) {
    if (size_ >= capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_cap);
    }
    data_[size_++] = std::move(value);
}

template<typename T>
void Stack<T>::pop() {
    if (size_ > 0) {
        --size_;
    }
}

template<typename T>
void Stack<T>::clear() noexcept {
    size_ = 0;
}

template<typename T>
void Stack<T>::swap(Stack& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

// ==================== 私有辅助 ====================

template<typename T>
void Stack<T>::reallocate_(std::size_t new_cap) {
    T* new_data = new T[new_cap];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    data_     = new_data;
    capacity_ = new_cap;
}

} // namespace dsc

#endif // DSC_STACK_H
