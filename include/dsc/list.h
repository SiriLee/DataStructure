#ifndef DSC_LIST_H
#define DSC_LIST_H

#include <cstddef>
#include <utility>

namespace dsc {

// ============================================================
// List<T> — 双向链表
// 支持头尾插入/删除，任意位置插入/删除
// ============================================================
template<typename T>
class List {
private:
    // 节点结构
    struct Node {
        T     data;
        Node* prev;
        Node* next;

        Node(const T& d, Node* p = nullptr, Node* n = nullptr)
            : data(d), prev(p), next(n) {}
        Node(T&& d, Node* p = nullptr, Node* n = nullptr)
            : data(std::move(d)), prev(p), next(n) {}
    };

    Node*       head_ = nullptr;
    Node*       tail_ = nullptr;
    std::size_t size_ = 0;

    // 返回第 pos 个节点（0-indexed），调用者保证 pos 在有效范围内
    Node* get_node_at_(std::size_t pos) const;

public:
    // 构造 / 析构
    List() = default;
    explicit List(std::size_t count, const T& value = T{});
    List(const List& other);
    List(List&& other) noexcept;
    ~List();

    // 赋值
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    // 容量
    bool        empty() const noexcept;
    std::size_t size() const noexcept;

    // 元素访问（要求非空）
    T&       front() noexcept;
    const T& front() const noexcept;
    T&       back() noexcept;
    const T& back() const noexcept;

    // 修改
    void clear() noexcept;
    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front();
    void pop_back();
    void insert(std::size_t pos, const T& value);
    void insert(std::size_t pos, T&& value);
    void erase(std::size_t pos);

    // 交换
    void swap(List& other) noexcept;
};

// ==================== 构造 / 析构 ====================

template<typename T>
List<T>::List(std::size_t count, const T& value) {
    for (std::size_t i = 0; i < count; ++i) {
        push_back(value);
    }
}

template<typename T>
List<T>::List(const List& other) {
    Node* cur = other.head_;
    while (cur) {
        push_back(cur->data);
        cur = cur->next;
    }
}

template<typename T>
List<T>::List(List&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template<typename T>
List<T>::~List() {
    clear();
}

// ==================== 赋值 ====================

template<typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        List temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

// ==================== 容量 ====================

template<typename T>
bool List<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
std::size_t List<T>::size() const noexcept {
    return size_;
}

// ==================== 元素访问 ====================

template<typename T>
T& List<T>::front() noexcept {
    return head_->data;
}

template<typename T>
const T& List<T>::front() const noexcept {
    return head_->data;
}

template<typename T>
T& List<T>::back() noexcept {
    return tail_->data;
}

template<typename T>
const T& List<T>::back() const noexcept {
    return tail_->data;
}

// ==================== 修改 ====================

template<typename T>
void List<T>::clear() noexcept {
    while (head_) {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
}

template<typename T>
void List<T>::push_front(const T& value) {
    Node* new_node = new Node(value, nullptr, head_);
    if (head_) {
        head_->prev = new_node;
    } else {
        tail_ = new_node;  // 原链表为空，新节点也是尾节点
    }
    head_ = new_node;
    ++size_;
}

template<typename T>
void List<T>::push_front(T&& value) {
    Node* new_node = new Node(std::move(value), nullptr, head_);
    if (head_) {
        head_->prev = new_node;
    } else {
        tail_ = new_node;
    }
    head_ = new_node;
    ++size_;
}

template<typename T>
void List<T>::push_back(const T& value) {
    Node* new_node = new Node(value, tail_, nullptr);
    if (tail_) {
        tail_->next = new_node;
    } else {
        head_ = new_node;  // 原链表为空，新节点也是头节点
    }
    tail_ = new_node;
    ++size_;
}

template<typename T>
void List<T>::push_back(T&& value) {
    Node* new_node = new Node(std::move(value), tail_, nullptr);
    if (tail_) {
        tail_->next = new_node;
    } else {
        head_ = new_node;
    }
    tail_ = new_node;
    ++size_;
}

template<typename T>
void List<T>::pop_front() {
    if (!head_) return;
    Node* old_head = head_;
    head_ = head_->next;
    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;  // 链表变空
    }
    delete old_head;
    --size_;
}

template<typename T>
void List<T>::pop_back() {
    if (!tail_) return;
    Node* old_tail = tail_;
    tail_ = tail_->prev;
    if (tail_) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;  // 链表变空
    }
    delete old_tail;
    --size_;
}

template<typename T>
void List<T>::insert(std::size_t pos, const T& value) {
    if (pos == 0) {
        push_front(value);
        return;
    }
    if (pos >= size_) {
        push_back(value);
        return;
    }
    Node* pos_node = get_node_at_(pos);
    Node* new_node = new Node(value, pos_node->prev, pos_node);
    pos_node->prev->next = new_node;
    pos_node->prev = new_node;
    ++size_;
}

template<typename T>
void List<T>::insert(std::size_t pos, T&& value) {
    if (pos == 0) {
        push_front(std::move(value));
        return;
    }
    if (pos >= size_) {
        push_back(std::move(value));
        return;
    }
    Node* pos_node = get_node_at_(pos);
    Node* new_node = new Node(std::move(value), pos_node->prev, pos_node);
    pos_node->prev->next = new_node;
    pos_node->prev = new_node;
    ++size_;
}

template<typename T>
void List<T>::erase(std::size_t pos) {
    if (pos >= size_) return;
    Node* node = get_node_at_(pos);
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head_ = node->next;  // 删除的是头节点
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail_ = node->prev;  // 删除的是尾节点
    }
    delete node;
    --size_;
}

// ==================== 交换 ====================

template<typename T>
void List<T>::swap(List& other) noexcept {
    using std::swap;
    swap(head_, other.head_);
    swap(tail_, other.tail_);
    swap(size_, other.size_);
}

// ==================== 私有辅助 ====================

template<typename T>
typename List<T>::Node* List<T>::get_node_at_(std::size_t pos) const {
    Node* cur = head_;
    for (std::size_t i = 0; i < pos; ++i) {
        cur = cur->next;
    }
    return cur;
}

} // namespace dsc

#endif // DSC_LIST_H
