#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
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

        Iterator& operator++()
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

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--()
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

        Iterator operator--(int)
        {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const
        {
            if (!current)
                throw out_of_range("Dereferencing null iterator");
            return current->data;
        }

        pointer operator->() const
        {
            if (!current)
                throw out_of_range("Dereferencing null iterator");
            return &current->data;
        }

        bool operator==(const Iterator& other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
    };

    using iterator = Iterator<false, false>;
    using const_iterator = Iterator<true, false>;
    using reverse_iterator = Iterator<false, true>;
    using const_reverse_iterator = Iterator<true, true>;

    LinkedList() : head(nullptr), tail(nullptr), list_size(0)
    {
    }

    LinkedList(const LinkedList& other) : LinkedList()
    {
        for (const auto& item : other) {
            push_back(item);
        }
    }

    LinkedList(LinkedList&& other) noexcept
        : head(other.head), tail(other.tail), list_size(other.list_size)
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }

    LinkedList(initializer_list<T> ilist) : LinkedList()
    {
        for (const auto& item : ilist) {
            push_back(item);
        }
    }

    template <typename InputIt>
    LinkedList(InputIt first, InputIt last)
        : head(nullptr), tail(nullptr), list_size(0)
    {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    ~LinkedList()
    {
        clear();
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (this != &other) {
            LinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    LinkedList& operator=(LinkedList&& other) noexcept
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

    T& front()
    {
        if (empty())
            throw out_of_range("List is empty");
        return head->data;
    }

    const T& front() const
    {
        if (empty())
            throw out_of_range("List is empty");
        return head->data;
    }

    T& back()
    {
        if (empty())
            throw out_of_range("List is empty");
        return tail->data;
    }

    const T& back() const
    {
        if (empty())
            throw out_of_range("List is empty");
        return tail->data;
    }

    void push_front(const T& value)
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

    void push_back(const T& value)
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

    void pop_front()
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

    void pop_back()
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

    iterator begin() noexcept
    {
        return iterator(head, this);
    }
    const_iterator begin() const noexcept
    {
        return const_iterator(head, this);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(head, this);
    }

    iterator end() noexcept
    {
        return iterator(nullptr, this);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(nullptr, this);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr, this);
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(tail, this);
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(tail, this);
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(tail, this);
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(nullptr, this);
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(nullptr, this);
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(nullptr, this);
    }

    bool empty() const noexcept
    {
        return list_size == 0;
    }
    size_t size() const noexcept
    {
        return list_size;
    }

    void clear()
    {
        while (!empty()) {
            pop_front();
        }
    }

    void swap(LinkedList& other) noexcept
    {
        swap(head, other.head);
        swap(tail, other.tail);
        swap(list_size, other.list_size);
    }
};

template <typename T>
void print_list(const LinkedList<T>& list)
{
    for (const auto& item : list) {
        cout << item << " ";
    }
    cout << "\n";
}

void test_stl_algorithms()
{
    LinkedList<int> list = {5, 3, 1, 4, 2};

    auto it = find(list.begin(), list.end(), 4);
    if (it != list.end()) {
        cout << "Found value 4 in the list\n";
    } else {
        cout << "Value 4 not found in the list\n";
    }

    cout << "Original list: ";
    print_list(list);

    if (list.size() > 1) {
        auto first = list.begin();
        auto last = list.end();
        --last;

        while (first != last) {
            swap(*first, *last);
            ++first;
            if (first == last)
                break;
            --last;
        }
    }

    cout << "After reverse: ";
    print_list(list);

    if (list.size() > 1) {
        bool swapped;
        do {
            swapped = false;
            auto it = list.begin();
            auto next = it;
            ++next;

            while (next != list.end()) {
                if (*it > *next) {
                    swap(*it, *next);
                    swapped = true;
                }
                ++it;
                ++next;
            }
        } while (swapped);
    }

    cout << "After sort: ";
    print_list(list);
}

int main()
{
    try {
        test_stl_algorithms();

        LinkedList<int> original_list = {1, 2, 3, 4, 5};
        LinkedList<int> range_constructed_list(
                original_list.begin(), original_list.end());

        cout << "Original list: ";
        print_list(original_list);

        cout << "Range constructed list: ";
        print_list(range_constructed_list);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}