#ifndef DSC_VECTOR_H
#define DSC_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace dsc {

// ============================================================
// Vector<T> — 动态数组
// 自动扩容，支持随机访问
// ============================================================
template<typename T>
class Vector {
public:
    // 构造 / 析构
    Vector() = default;
    Vector(std::initializer_list<T> list);
    explicit Vector(std::size_t count, const T& value = T{});
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    // 赋值
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    // 元素访问（无边界检查）
    T& operator[](std::size_t pos) noexcept;
    const T& operator[](std::size_t pos) const noexcept;

    // 元素访问（带边界检查）
    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;

    // 首尾元素访问
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // 直接访问底层数组
    T* data() noexcept;
    const T* data() const noexcept;

    // 容量
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);
    void resize(std::size_t new_size, const T& value = T{});
    void shrink_to_fit();

    // 修改
    void clear() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void insert(std::size_t pos, const T& value);
    void insert(std::size_t pos, T&& value);
    void erase(std::size_t pos);

    void swap(Vector& other) noexcept;

private:
    std::size_t size_     = 0;
    std::size_t capacity_ = 0;
    T*          data_     = nullptr;

    // 私有辅助函数
    void reallocate_(std::size_t new_capacity);
    void check_and_grow_();
};

// ==================== 构造 / 析构 ====================

template<typename T>
Vector<T>::Vector(std::initializer_list<T> list)
    : size_(list.size()), capacity_(list.size()), data_(new T[list.size()]) {
    std::copy(list.begin(), list.end(), data_);
}

template<typename T>
Vector<T>::Vector(std::size_t count, const T& value)
    : size_(count), capacity_(count), data_(new T[count]) {
    for (std::size_t i = 0; i < count; ++i) {
        data_[i] = value;
    }
}

template<typename T>
Vector<T>::Vector(const Vector& other)
    : size_(other.size_), capacity_(other.capacity_), data_(new T[other.capacity_]) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.size_     = 0;
    other.capacity_ = 0;
    other.data_     = nullptr;
}

template<typename T>
Vector<T>::~Vector() {
    delete[] data_;
}

// ==================== 赋值 ====================

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        size_     = other.size_;
        capacity_ = other.capacity_;
        data_     = other.data_;
        other.size_     = 0;
        other.capacity_ = 0;
        other.data_     = nullptr;
    }
    return *this;
}

// ==================== 元素访问 ====================

template<typename T>
T& Vector<T>::operator[](std::size_t pos) noexcept {
    return data_[pos];
}

template<typename T>
const T& Vector<T>::operator[](std::size_t pos) const noexcept {
    return data_[pos];
}

template<typename T>
T& Vector<T>::at(std::size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

template<typename T>
const T& Vector<T>::at(std::size_t pos) const {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

template<typename T>
T& Vector<T>::front() {
    return data_[0];
}

template<typename T>
const T& Vector<T>::front() const {
    return data_[0];
}

template<typename T>
T& Vector<T>::back() {
    return data_[size_ - 1];
}

template<typename T>
const T& Vector<T>::back() const {
    return data_[size_ - 1];
}

template<typename T>
T* Vector<T>::data() noexcept {
    return data_;
}

template<typename T>
const T* Vector<T>::data() const noexcept {
    return data_;
}

// ==================== 容量 ====================

template<typename T>
bool Vector<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
std::size_t Vector<T>::size() const noexcept {
    return size_;
}

template<typename T>
std::size_t Vector<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
void Vector<T>::reserve(std::size_t new_cap) {
    if (new_cap > capacity_) {
        reallocate_(new_cap);
    }
}

template<typename T>
void Vector<T>::resize(std::size_t new_size, const T& value) {
    if (new_size > capacity_) {
        reallocate_(std::max(new_size, capacity_ * 2));
    }
    for (std::size_t i = size_; i < new_size; ++i) {
        data_[i] = value;
    }
    size_ = new_size;
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    if (capacity_ > size_) {
        reallocate_(size_);
    }
}

// ==================== 修改 ====================

template<typename T>
void Vector<T>::clear() noexcept {
    size_ = 0;
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    check_and_grow_();
    data_[size_++] = value;
}

template<typename T>
void Vector<T>::push_back(T&& value) {
    check_and_grow_();
    data_[size_++] = std::move(value);
}

template<typename T>
void Vector<T>::pop_back() {
    if (size_ > 0) {
        --size_;
    }
}

template<typename T>
void Vector<T>::insert(std::size_t pos, const T& value) {
    if (pos > size_) {
        throw std::out_of_range("Vector::insert: index out of range");
    }
    check_and_grow_();
    // 将 [pos, size_-1] 的元素整体后移一位
    for (std::size_t i = size_; i > pos; --i) {
        data_[i] = data_[i - 1];
    }
    data_[pos] = value;
    ++size_;
}

template<typename T>
void Vector<T>::insert(std::size_t pos, T&& value) {
    if (pos > size_) {
        throw std::out_of_range("Vector::insert: index out of range");
    }
    check_and_grow_();
    for (std::size_t i = size_; i > pos; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[pos] = std::move(value);
    ++size_;
}

template<typename T>
void Vector<T>::erase(std::size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range("Vector::erase: index out of range");
    }
    // 将 [pos+1, size_-1] 的元素前移，覆盖 pos 位置
    for (std::size_t i = pos; i < size_ - 1; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
}

template<typename T>
void Vector<T>::swap(Vector& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

// ==================== 私有辅助 ====================

template<typename T>
void Vector<T>::reallocate_(std::size_t new_capacity) {
    T* new_data = new T[new_capacity];
    std::size_t count = std::min(new_capacity, size_);
    for (std::size_t i = 0; i < count; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    data_     = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void Vector<T>::check_and_grow_() {
    if (size_ >= capacity_) {
        std::size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate_(new_cap);
    }
}

} // namespace dsc

#endif // DSC_VECTOR_H
