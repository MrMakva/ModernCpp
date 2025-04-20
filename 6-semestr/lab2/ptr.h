#include <atomic>
using namespace std;

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr;

template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    explicit UniquePtr(T* p = nullptr) : ptr(p)
    {
    }

    ~UniquePtr()
    {
        delete ptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const
    {
        return *ptr;
    }
    T* operator->() const
    {
        return ptr;
    }
    T* get() const
    {
        return ptr;
    }

    void reset(T* p = nullptr)
    {
        delete ptr;
        ptr = p;
    }
};

template <typename T>
class SharedPtr {
private:
    T* ptr;
    atomic<size_t>* ref_count;

    friend class WeakPtr<T>;

public:
    explicit SharedPtr(T* p = nullptr)
        : ptr(p), ref_count(new atomic<size_t>(p ? 1 : 0))
    {
    }

    ~SharedPtr()
    {
        if (ref_count && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }

    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count)
    {
        if (ref_count)
            ++(*ref_count);
    }

    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count)
                ++(*ref_count);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count)
    {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept
    {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    explicit SharedPtr(const WeakPtr<T>& weak)
        : ptr(weak.ptr), ref_count(weak.ref_count)
    {
        if (ref_count)
            ++(*ref_count);
    }

    T& operator*() const
    {
        return *ptr;
    }
    T* operator->() const
    {
        return ptr;
    }
    T* get() const
    {
        return ptr;
    }
    size_t use_count() const
    {
        return ref_count ? ref_count->load() : 0;
    }
};

template <typename T>
class WeakPtr {
private:
    T* ptr;
    atomic<size_t>* ref_count;

    friend class SharedPtr<T>;

public:
    WeakPtr() : ptr(nullptr), ref_count(nullptr)
    {
    }

    WeakPtr(const SharedPtr<T>& shared)
        : ptr(shared.ptr), ref_count(shared.ref_count)
    {
    }

    size_t use_count() const
    {
        return ref_count ? ref_count->load() : 0;
    }

    bool expired() const
    {
        return use_count() == 0;
    }

    SharedPtr<T> lock() const
    {
        return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
    }
};