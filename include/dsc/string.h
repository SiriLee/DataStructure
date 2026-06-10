#ifndef DSC_STRING_H
#define DSC_STRING_H

#include <cstddef>
#include <cstring>
#include <utility>
#include <stdexcept>
#include <iostream>

namespace dsc {

// ============================================================
// String — 字符串类
// 类似 std::string 的简化实现
// ============================================================
class String {
public:
    // 构造 / 析构
    String() noexcept;
    String(const char* s);
    String(std::size_t count, char ch);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // 赋值
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String& operator=(const char* s);
    String& operator=(char ch);

    // 元素访问
    char&       operator[](std::size_t pos) noexcept;
    const char& operator[](std::size_t pos) const noexcept;
    char&       at(std::size_t pos);
    const char& at(std::size_t pos) const;
    char&       front() noexcept;
    const char& front() const noexcept;
    char&       back() noexcept;
    const char& back() const noexcept;
    char*       data() noexcept;
    const char* data() const noexcept;
    const char* c_str() const noexcept;

    // 容量
    bool        empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t length() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);
    void resize(std::size_t count, char ch = '\0');
    void shrink_to_fit();

    // 修改
    void    clear() noexcept;
    void    push_back(char ch);
    void    pop_back();
    String& append(const String& str);
    String& append(const char* s);
    String& append(std::size_t count, char ch);
    String& append(char ch);
    String& operator+=(const String& str);
    String& operator+=(const char* s);
    String& operator+=(char ch);
    String& insert(std::size_t pos, const String& str);
    String& insert(std::size_t pos, const char* s);
    String& insert(std::size_t pos, std::size_t count, char ch);
    String& erase(std::size_t pos = 0, std::size_t count = npos);
    void    swap(String& other) noexcept;

    // 查找
    std::size_t find(const String& str, std::size_t pos = 0) const noexcept;
    std::size_t find(const char* s, std::size_t pos = 0) const noexcept;
    std::size_t find(char ch, std::size_t pos = 0) const noexcept;
    std::size_t rfind(const String& str, std::size_t pos = npos) const noexcept;
    std::size_t rfind(const char* s, std::size_t pos = npos) const noexcept;
    std::size_t rfind(char ch, std::size_t pos = npos) const noexcept;

    // 比较
    int compare(const String& str) const noexcept;
    int compare(const char* s) const noexcept;

    // 静态成员
    static const std::size_t npos = static_cast<std::size_t>(-1);

private:
    char*       data_     = nullptr;
    std::size_t size_     = 0;
    std::size_t capacity_ = 0;  // 包含 '\0' 的空间

    void reallocate_(std::size_t new_cap);  // new_cap 包含 '\0'
};

// ==================== 构造 / 析构 ====================

inline String::String() noexcept
    : data_(new char[1]()), size_(0), capacity_(1) {}

inline String::String(const char* s) {
    if (!s) s = "";
    std::size_t len = std::strlen(s);
    data_     = new char[len + 1];
    size_     = len;
    capacity_ = len + 1;
    std::memcpy(data_, s, len + 1);
}

inline String::String(std::size_t count, char ch)
    : data_(new char[count + 1]), size_(count), capacity_(count + 1) {
    for (std::size_t i = 0; i < count; ++i) {
        data_[i] = ch;
    }
    data_[count] = '\0';
}

inline String::String(const String& other)
    : data_(new char[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
    std::memcpy(data_, other.data_, size_ + 1);
}

inline String::String(String&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_     = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
}

inline String::~String() {
    delete[] data_;
}

// ==================== 赋值 ====================

inline String& String::operator=(const String& other) {
    if (this != &other) {
        String temp(other);
        swap(temp);
    }
    return *this;
}

inline String& String::operator=(String&& other) noexcept {
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

inline String& String::operator=(const char* s) {
    String temp(s);
    swap(temp);
    return *this;
}

inline String& String::operator=(char ch) {
    String temp(1, ch);
    swap(temp);
    return *this;
}

// ==================== 元素访问 ====================

inline char& String::operator[](std::size_t pos) noexcept {
    return data_[pos];
}

inline const char& String::operator[](std::size_t pos) const noexcept {
    return data_[pos];
}

inline char& String::at(std::size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range("String::at: index out of range");
    }
    return data_[pos];
}

inline const char& String::at(std::size_t pos) const {
    if (pos >= size_) {
        throw std::out_of_range("String::at: index out of range");
    }
    return data_[pos];
}

inline char& String::front() noexcept {
    return data_[0];
}

inline const char& String::front() const noexcept {
    return data_[0];
}

inline char& String::back() noexcept {
    return data_[size_ - 1];
}

inline const char& String::back() const noexcept {
    return data_[size_ - 1];
}

inline char* String::data() noexcept {
    return data_;
}

inline const char* String::data() const noexcept {
    return data_;
}

inline const char* String::c_str() const noexcept {
    return data_;
}

// ==================== 容量 ====================

inline bool String::empty() const noexcept {
    return size_ == 0;
}

inline std::size_t String::size() const noexcept {
    return size_;
}

inline std::size_t String::length() const noexcept {
    return size_;
}

inline std::size_t String::capacity() const noexcept {
    return capacity_ > 0 ? capacity_ - 1 : 0;  // 不含 '\0' 的空间
}

inline void String::reserve(std::size_t new_cap) {
    // new_cap 为不含 '\0' 的容量
    if (new_cap > capacity()) {
        reallocate_(new_cap + 1);
    }
}

inline void String::resize(std::size_t count, char ch) {
    if (count <= size_) {
        size_ = count;
        data_[size_] = '\0';
    } else {
        if (count + 1 > capacity_) {
            reserve(count);
        }
        for (std::size_t i = size_; i < count; ++i) {
            data_[i] = ch;
        }
        size_ = count;
        data_[size_] = '\0';
    }
}

inline void String::shrink_to_fit() {
    if (capacity_ > size_ + 1) {
        reallocate_(size_ + 1);
    }
}

// ==================== 修改 ====================

inline void String::clear() noexcept {
    size_ = 0;
    data_[0] = '\0';
}

inline void String::push_back(char ch) {
    if (size_ + 1 >= capacity_) {
        std::size_t new_cap = (capacity_ <= 1) ? 2 : capacity_ * 2;
        reallocate_(new_cap);
    }
    data_[size_++] = ch;
    data_[size_] = '\0';
}

inline void String::pop_back() {
    if (size_ > 0) {
        --size_;
        data_[size_] = '\0';
    }
}

inline String& String::append(const String& str) {
    return append(str.data_);
}

inline String& String::append(const char* s) {
    if (!s) return *this;
    std::size_t len = std::strlen(s);
    if (len == 0) return *this;
    if (size_ + len + 1 > capacity_) {
        reserve(size_ + len);
    }
    std::memcpy(data_ + size_, s, len + 1);
    size_ += len;
    return *this;
}

inline String& String::append(std::size_t count, char ch) {
    if (count == 0) return *this;
    if (size_ + count + 1 > capacity_) {
        reserve(size_ + count);
    }
    for (std::size_t i = 0; i < count; ++i) {
        data_[size_ + i] = ch;
    }
    size_ += count;
    data_[size_] = '\0';
    return *this;
}

inline String& String::append(char ch) {
    push_back(ch);
    return *this;
}

inline String& String::operator+=(const String& str) {
    return append(str);
}

inline String& String::operator+=(const char* s) {
    return append(s);
}

inline String& String::operator+=(char ch) {
    return append(ch);
}

inline String& String::insert(std::size_t pos, const String& str) {
    return insert(pos, str.data_);
}

inline String& String::insert(std::size_t pos, const char* s) {
    if (pos > size_) throw std::out_of_range("String::insert: index out of range");
    if (!s) return *this;
    std::size_t len = std::strlen(s);
    if (len == 0) return *this;
    if (size_ + len + 1 > capacity_) {
        reserve(size_ + len);
    }
    // 后移 [pos, size_] 段（包含 '\0'）
    std::memmove(data_ + pos + len, data_ + pos, size_ - pos + 1);
    std::memcpy(data_ + pos, s, len);
    size_ += len;
    return *this;
}

inline String& String::insert(std::size_t pos, std::size_t count, char ch) {
    if (pos > size_) throw std::out_of_range("String::insert: index out of range");
    if (count == 0) return *this;
    if (size_ + count + 1 > capacity_) {
        reserve(size_ + count);
    }
    std::memmove(data_ + pos + count, data_ + pos, size_ - pos + 1);
    for (std::size_t i = 0; i < count; ++i) {
        data_[pos + i] = ch;
    }
    size_ += count;
    return *this;
}

inline String& String::erase(std::size_t pos, std::size_t count) {
    if (pos > size_) throw std::out_of_range("String::erase: index out of range");
    if (count == npos || pos + count > size_) {
        count = size_ - pos;
    }
    if (count == 0) return *this;
    // 将 [pos+count, size_] 段前移（包含 '\0'）
    std::memmove(data_ + pos, data_ + pos + count, size_ - pos - count + 1);
    size_ -= count;
    return *this;
}

inline void String::swap(String& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

// ==================== 查找 ====================

inline std::size_t String::find(const String& str, std::size_t pos) const noexcept {
    return find(str.data_, pos);
}

inline std::size_t String::find(const char* s, std::size_t pos) const noexcept {
    if (!s || pos > size_) return npos;
    const char* found = std::strstr(data_ + pos, s);
    return found ? static_cast<std::size_t>(found - data_) : npos;
}

inline std::size_t String::find(char ch, std::size_t pos) const noexcept {
    if (pos >= size_) return npos;
    const char* found = static_cast<const char*>(std::memchr(data_ + pos, ch, size_ - pos));
    return found ? static_cast<std::size_t>(found - data_) : npos;
}

inline std::size_t String::rfind(const String& str, std::size_t pos) const noexcept {
    return rfind(str.data_, pos);
}

inline std::size_t String::rfind(const char* s, std::size_t pos) const noexcept {
    if (!s) return npos;
    std::size_t len = std::strlen(s);
    if (len == 0) return (pos < size_) ? pos : size_;
    if (len > size_) return npos;
    std::size_t start = (pos >= size_ - len) ? size_ - len : pos;
    for (std::size_t i = start; i != npos; --i) {
        if (std::memcmp(data_ + i, s, len) == 0) return i;
        if (i == 0) break;
    }
    return npos;
}

inline std::size_t String::rfind(char ch, std::size_t pos) const noexcept {
    if (size_ == 0) return npos;
    std::size_t start = (pos >= size_) ? size_ - 1 : pos;
    for (std::size_t i = start; i != npos; --i) {
        if (data_[i] == ch) return i;
        if (i == 0) break;
    }
    return npos;
}

// ==================== 比较 ====================

inline int String::compare(const String& str) const noexcept {
    return std::strcmp(data_, str.data_);
}

inline int String::compare(const char* s) const noexcept {
    if (!s) return (size_ == 0) ? 0 : 1;
    return std::strcmp(data_, s);
}

// ==================== 私有辅助 ====================

inline void String::reallocate_(std::size_t new_cap) {
    // new_cap 是包含 '\0' 的总容量
    char* new_data = new char[new_cap];
    std::memcpy(new_data, data_, (size_ < new_cap ? size_ : new_cap - 1) + 1);
    delete[] data_;
    data_     = new_data;
    capacity_ = new_cap;
}

// ==================== 非成员运算符 ====================

inline bool operator==(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) == 0;
}

inline bool operator!=(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) != 0;
}

inline bool operator<(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) < 0;
}

inline bool operator<=(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) <= 0;
}

inline bool operator>(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) > 0;
}

inline bool operator>=(const String& lhs, const String& rhs) noexcept {
    return lhs.compare(rhs) >= 0;
}

inline std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

} // namespace dsc

#endif // DSC_STRING_H
