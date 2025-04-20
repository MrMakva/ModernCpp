#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
using namespace std;

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value, Node* p = nullptr, Node* n = nullptr)
            : data(value), prev(p), next(n)
        {
        }
    };

    Node* head;
    Node* tail;
    size_t list_size;

public:
    template <bool IsConst, bool IsReverse>
    class Iterator {
    private:
        friend class LinkedList;
        using node_ptr = conditional_t<IsConst, const Node*, Node*>;
        node_ptr current;
        const LinkedList* parent;

    public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = conditional_t<IsConst, const T*, T*>;
        using reference = conditional_t<IsConst, const T&, T&>;

        Iterator() : current(nullptr), parent(nullptr)
        {
        }
        Iterator(node_ptr node, const LinkedList* p) : current(node), parent(p)
        {
        }

        operator Iterator<true, IsReverse>() const
        {
            return Iterator<true, IsReverse>(current, parent);
        }

        Iterator& operator++();

        Iterator operator++(int);

        Iterator& operator--();

        Iterator operator--(int);

        reference operator*() const;

        pointer operator->() const;

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    using iterator = Iterator<false, false>;
    using const_iterator = Iterator<true, false>;
    using reverse_iterator = Iterator<false, true>;
    using const_reverse_iterator = Iterator<true, true>;

    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList(LinkedList&& other) noexcept;
    LinkedList(initializer_list<T> ilist);
    ~LinkedList();

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other) noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front();
    void pop_back();
    void clear();
    void swap(LinkedList& other) noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
};

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>&
LinkedList<T>::Iterator<IsConst, IsReverse>::operator++()
{
    if (!current) {
        throw out_of_range("Cannot increment end iterator");
    }
    if constexpr (IsReverse) {
        current = current->prev;
    } else {
        current = current->next;
    }
    return *this;
}

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>
LinkedList<T>::Iterator<IsConst, IsReverse>::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>&
LinkedList<T>::Iterator<IsConst, IsReverse>::operator--()
{
    if (!current) {
        if constexpr (IsReverse) {
            current = parent ? parent->tail : nullptr;
        } else {
            current = parent ? parent->head : nullptr;
        }
        if (!current) {
            throw out_of_range("Cannot decrement begin iterator");
        }
    } else {
        if constexpr (IsReverse) {
            current = current->next;
        } else {
            current = current->prev;
        }
        if (!current) {
            throw out_of_range("Cannot decrement begin iterator");
        }
    }
    return *this;
}

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>
LinkedList<T>::Iterator<IsConst, IsReverse>::operator--(int)
{
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>::reference
LinkedList<T>::Iterator<IsConst, IsReverse>::operator*() const
{
    if (!current)
        throw out_of_range("Dereferencing null iterator");
    return current->data;
}

template <typename T>
template <bool IsConst, bool IsReverse>
typename LinkedList<T>::template Iterator<IsConst, IsReverse>::pointer
LinkedList<T>::Iterator<IsConst, IsReverse>::operator->() const
{
    if (!current)
        throw out_of_range("Dereferencing null iterator");
    return &current->data;
}

template <typename T>
template <bool IsConst, bool IsReverse>
bool LinkedList<T>::Iterator<IsConst, IsReverse>::operator==(
        const Iterator& other) const
{
    return current == other.current;
}

template <typename T>
template <bool IsConst, bool IsReverse>
bool LinkedList<T>::Iterator<IsConst, IsReverse>::operator!=(
        const Iterator& other) const
{
    return !(*this == other);
}

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), list_size(0)
{
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : LinkedList()
{
    for (const auto& item : other) {
        push_back(item);
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
    : head(other.head), tail(other.tail), list_size(other.list_size)
{
    other.head = nullptr;
    other.tail = nullptr;
    other.list_size = 0;
}

template <typename T>
LinkedList<T>::LinkedList(initializer_list<T> ilist) : LinkedList()
{
    for (const auto& item : ilist) {
        push_back(item);
    }
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    clear();
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other)
{
    if (this != &other) {
        LinkedList tmp(other);
        swap(tmp);
    }
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept
{
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        list_size = other.list_size;
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }
    return *this;
}

template <typename T>
T& LinkedList<T>::front()
{
    if (empty())
        throw out_of_range("List is empty");
    return head->data;
}

template <typename T>
const T& LinkedList<T>::front() const
{
    if (empty())
        throw out_of_range("List is empty");
    return head->data;
}

template <typename T>
T& LinkedList<T>::back()
{
    if (empty())
        throw out_of_range("List is empty");
    return tail->data;
}

template <typename T>
const T& LinkedList<T>::back() const
{
    if (empty())
        throw out_of_range("List is empty");
    return tail->data;
}

template <typename T>
void LinkedList<T>::push_front(const T& value)
{
    Node* new_node = new Node(value, nullptr, head);
    if (head) {
        head->prev = new_node;
    } else {
        tail = new_node;
    }
    head = new_node;
    ++list_size;
}

template <typename T>
void LinkedList<T>::push_back(const T& value)
{
    Node* new_node = new Node(value, tail, nullptr);
    if (tail) {
        tail->next = new_node;
    } else {
        head = new_node;
    }
    tail = new_node;
    ++list_size;
}

template <typename T>
void LinkedList<T>::pop_front()
{
    if (empty())
        return;
    Node* to_delete = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete to_delete;
    --list_size;
}

template <typename T>
void LinkedList<T>::pop_back()
{
    if (empty())
        return;
    Node* to_delete = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete to_delete;
    --list_size;
}

template <typename T>
void LinkedList<T>::clear()
{
    while (!empty()) {
        pop_front();
    }
}

template <typename T>
void LinkedList<T>::swap(LinkedList& other) noexcept
{
    swap(head, other.head);
    swap(tail, other.tail);
    swap(list_size, other.list_size);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() noexcept
{
    return iterator(head, this);
}

template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const noexcept
{
    return const_iterator(head, this);
}

template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const noexcept
{
    return const_iterator(head, this);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() noexcept
{
    return iterator(nullptr, this);
}

template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const noexcept
{
    return const_iterator(nullptr, this);
}

template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const noexcept
{
    return const_iterator(nullptr, this);
}

template <typename T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::rbegin() noexcept
{
    return reverse_iterator(tail, this);
}

template <typename T>
typename LinkedList<T>::const_reverse_iterator
LinkedList<T>::rbegin() const noexcept
{
    return const_reverse_iterator(tail, this);
}

template <typename T>
typename LinkedList<T>::const_reverse_iterator
LinkedList<T>::crbegin() const noexcept
{
    return const_reverse_iterator(tail, this);
}

template <typename T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::rend() noexcept
{
    return reverse_iterator(nullptr, this);
}

template <typename T>
typename LinkedList<T>::const_reverse_iterator
LinkedList<T>::rend() const noexcept
{
    return const_reverse_iterator(nullptr, this);
}

template <typename T>
typename LinkedList<T>::const_reverse_iterator
LinkedList<T>::crend() const noexcept
{
    return const_reverse_iterator(nullptr, this);
}

template <typename T>
bool LinkedList<T>::empty() const noexcept
{
    return list_size == 0;
}

template <typename T>
size_t LinkedList<T>::size() const noexcept
{
    return list_size;
}

template <typename T>
void swap(LinkedList<T>& lhs, LinkedList<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

#endif