#ifndef DSC_ARRAY_H
#define DSC_ARRAY_H

#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

namespace dsc {

// ============================================================
// Array<T, N> — 固定大小数组
// 编译期确定大小，行为类似 std::array
// ============================================================
template<typename T, std::size_t N>
class Array {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;

    // 构造/赋值/析构（均采用默认实现）
    Array() = default;
    Array(std::initializer_list<T> list);
    Array(const Array&) = default;
    Array(Array&&) = default;
    Array& operator=(const Array&) = default;
    Array& operator=(Array&&) = default;
    ~Array() = default;

    // 元素访问（无边界检查）
    reference operator[](size_type pos) noexcept;
    const_reference operator[](size_type pos) const noexcept;

    // 元素访问（带边界检查）
    reference at(size_type pos);
    const_reference at(size_type pos) const;

    // 首尾元素访问
    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;

    // 直接访问底层数组
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // 容量操作
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr bool empty() const noexcept;

    // 数据操作
    void fill(const T& value);
    void swap(Array& other) noexcept;

private:
    T elems_[N] = {};
};

// ==================== 实现 ====================

template<typename T, std::size_t N>
Array<T, N>::Array(std::initializer_list<T> list) {
    size_type count = std::min(list.size(), N);
    std::copy(list.begin(), list.begin() + count, elems_);
}

template<typename T, std::size_t N>
typename Array<T, N>::reference
Array<T, N>::operator[](size_type pos) noexcept {
    return elems_[pos];
}

template<typename T, std::size_t N>
typename Array<T, N>::const_reference
Array<T, N>::operator[](size_type pos) const noexcept {
    return elems_[pos];
}

template<typename T, std::size_t N>
typename Array<T, N>::reference
Array<T, N>::at(size_type pos) {
    if (pos >= N) {
        throw std::out_of_range("Array::at: index out of range");
    }
    return elems_[pos];
}

template<typename T, std::size_t N>
typename Array<T, N>::const_reference
Array<T, N>::at(size_type pos) const {
    if (pos >= N) {
        throw std::out_of_range("Array::at: index out of range");
    }
    return elems_[pos];
}

template<typename T, std::size_t N>
typename Array<T, N>::reference
Array<T, N>::front() noexcept {
    return elems_[0];
}

template<typename T, std::size_t N>
typename Array<T, N>::const_reference
Array<T, N>::front() const noexcept {
    return elems_[0];
}

template<typename T, std::size_t N>
typename Array<T, N>::reference
Array<T, N>::back() noexcept {
    return elems_[N - 1];
}

template<typename T, std::size_t N>
typename Array<T, N>::const_reference
Array<T, N>::back() const noexcept {
    return elems_[N - 1];
}

template<typename T, std::size_t N>
typename Array<T, N>::pointer
Array<T, N>::data() noexcept {
    return elems_;
}

template<typename T, std::size_t N>
typename Array<T, N>::const_pointer
Array<T, N>::data() const noexcept {
    return elems_;
}

template<typename T, std::size_t N>
constexpr typename Array<T, N>::size_type
Array<T, N>::size() const noexcept {
    return N;
}

template<typename T, std::size_t N>
constexpr typename Array<T, N>::size_type
Array<T, N>::max_size() const noexcept {
    return N;
}

template<typename T, std::size_t N>
constexpr bool
Array<T, N>::empty() const noexcept {
    return N == 0;
}

template<typename T, std::size_t N>
void Array<T, N>::fill(const T& value) {
    for (size_type i = 0; i < N; ++i) {
        elems_[i] = value;
    }
}

template<typename T, std::size_t N>
void Array<T, N>::swap(Array& other) noexcept {
    for (size_type i = 0; i < N; ++i) {
        using std::swap;
        swap(elems_[i], other.elems_[i]);
    }
}

} // namespace dsc

#endif // DSC_ARRAY_H
