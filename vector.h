#include <cstddef>
#include <memory>
#include <iostream>
#include <concepts>
#include <iterator>

template<typename a, typename b>
concept same_size = sizeof(a) == sizeof(b);

template<typename Derived, typename Base>
concept emplaceable_from = same_size<Derived, Base> && std::derived_from<Derived, Base>;

template<typename Base, typename Allocator = std::allocator<Base>> 
class PolyVector {
public:
    // Member types
    using value_type = Base;
    using allocator_type = Allocator;
    using size_type = size_t;
    using reference = Base&;
    using pointer = Base*;
    class iterator {
        public:
            // Member types
            using difference_type = std::ptrdiff_t;
            using value_type = Base;
            using reference = Base&;
            using pointer = Base*;
            using iterator_category = std::random_access_iterator_tag;

            // Member functions
            iterator() : mPtr{nullptr} {};
            iterator(Base* ptr) : mPtr{ptr} {};
            iterator(const iterator& other) : mPtr{other.mPtr} {};

            
            // operators
            Base& operator*() const {
                return *mPtr;
            }

            iterator& operator++() {
                ++mPtr;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const iterator other) const {
                return other.mPtr == mPtr;
            }

            bool operator!=(const iterator other) const {
                return other.mPtr != mPtr;
            }
            
        private:
            Base* mPtr;
    };

    // Member functions
    ~PolyVector();

    // Element access
    Base& operator[](size_t index);
    Base& front();
    Base& back();
    Base* data();

    // Iterators
    iterator begin() {
        return iterator(data_);
    }

    iterator end() {
        return iterator(data_ + size_);
    }

    // Capacity
    size_t size();
    void reserve(size_t new_capacity);

    // Modifiers
    void push_back(Base value);
    template <typename Derived, typename... Args>
    requires emplaceable_from<Derived, Base> 
    void emplace_back(Args&&... args);
    void pop_back();
    

private:
    Base* data_ {nullptr};
    size_t size_ {0};
    size_t capacity_ {0};

    void trusted_reserve(size_t new_capacity);
    void expand_if_full();
};

// private

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::trusted_reserve(size_t new_capacity) {

    Allocator allocator;
    Base* new_data = allocator.allocate(new_capacity);
    
    if (data_ != nullptr) {
        for (size_t index = 0; index < capacity_; index++) {
            new (new_data + index) Base{data_[index]};
        } 
        for (size_t index = 0; index < capacity_; index++) {
            data_[index].~Base();
        } 
        allocator.deallocate(data_, capacity_);
    }
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::expand_if_full() {
    if (size_ == capacity_) {
        trusted_reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
}

// Member functions
template<typename Base, typename Allocator> 
PolyVector<Base, Allocator>::~PolyVector() {
    if (data_ != nullptr) {
        Allocator allocator;
        for (size_t index = 0; index < capacity_; index++) {
            data_[index].~Base();
        } 
        allocator.deallocate(data_, capacity_);
    }
}

// Element access

template<typename Base, typename Allocator> 
Base& PolyVector<Base, Allocator>::operator[](size_t index) {
    return data_[index];
}

template<typename Base, typename Allocator> 
Base& PolyVector<Base, Allocator>::front() {
    return data_[0];
}

template<typename Base, typename Allocator> 
Base& PolyVector<Base, Allocator>::back() {
    return data_[size_ - 1];
}

template<typename Base, typename Allocator> 
Base* PolyVector<Base, Allocator>::data() {
    return data_;
}

// Capacity

template<typename Base, typename Allocator> 
size_t PolyVector<Base, Allocator>::size() {
    return size_;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        trusted_reserve(new_capacity);
    }
}

// Modifiers

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::push_back(Base value) {
    expand_if_full();
    new (data_ + size_) Base{value};
    size_++;
}


template<typename Base, typename Allocator> 
template<typename Derived, typename... Args>
requires emplaceable_from<Derived, Base>
void PolyVector<Base, Allocator>::emplace_back(Args&&... args) {
    expand_if_full();
    new (data_ + size_) Derived(std::forward<Args>(args)...);
    size_++;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::pop_back() {
    if (size_ > 0) {
        data_[size_ - 1].~Base();
        size_--;
    }
}
