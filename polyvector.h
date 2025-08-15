#ifndef POLYVECTOR_H
#define POLYVECTOR_H 

#include <cstddef>
#include <memory>
#include <iostream>
#include <concepts>
#include <iterator>
#include <cstring>
#include <initializer_list>

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
    PolyVector() = default;
    PolyVector(std::initializer_list<Base> init);

    PolyVector(PolyVector& other) = delete;
    void operator=(PolyVector& other) = delete;

    PolyVector(PolyVector&& other) = delete;
    void operator=(PolyVector&& other) = delete;

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
    size_t capacity();

    // Modifiers
    void clear();
    void insert(const iterator pos, const Base& value);
    void erase_value(const Base& value);
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
        std::memcpy(static_cast<void*>(new_data), static_cast<void*>(data_), sizeof(Base) * size_);
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
PolyVector<Base, Allocator>::PolyVector(std::initializer_list<Base> init) {
    for (Base item : init) {
        push_back(item);
    }
}

template<typename Base, typename Allocator> 
PolyVector<Base, Allocator>::~PolyVector() {
    if (data_ != nullptr) {
        Allocator allocator;
        for (size_t index = 0; index < size_; ++index) {
            data_[index].~Base();
        } 
        allocator.deallocate(data_, capacity_);
    }
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
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

template<typename Base, typename Allocator> 
size_t PolyVector<Base, Allocator>::capacity() {
    return capacity_;
}

// Modifiers

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::clear() {
    for (size_t index = 0; index < capacity_; ++index) {
        data_[index].~Base();
    }
    size_ = 0;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::insert(const iterator pos, const Base& value) {
    if (data_ == nullptr) {
        push_back(value);
        return;
    }

    if (size_ == capacity_) {
        size_t new_capacity = capacity_ * 2;
        Allocator allocator;
        Base* new_data = allocator.allocate(new_capacity);
        
        size_t insert_offset = std::addressof(*pos) - data_;

        std::memcpy(static_cast<void*>(new_data), static_cast<void*>(data_), sizeof(Base) * insert_offset);
        std::memmove(static_cast<void*>(new_data + insert_offset + 1), 
                    static_cast<void*>(data_ + insert_offset),
                    sizeof(Base) * (size_ - insert_offset));
        new (new_data + insert_offset) Base{value};
        allocator.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    else {
        size_t insert_offset = std::addressof(*pos) - data_;
        std::memmove(static_cast<void*>(data_ + insert_offset + 1), 
                    static_cast<void*>(data_ + insert_offset),
                    sizeof(Base) * (size_ - insert_offset));
        new (data_ + insert_offset) Base{value};
    }
    ++size_;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::erase_value(const Base& value) {
    size_t first;
    for (first = 0; first < size_; ++first) {
        if (data_[first] == value) {
            data_[first].~Base();
            break;        
        }
    }

    if (first == size_) {
        return;
    }

    size_t i;
    for(i = first + 1; i < size_; ++i) {
        if (data_[i] != value) {
            std::memcpy(static_cast<void*>(data_ + first),
                        static_cast<void*>(data_ + i),
                        sizeof(Base));
            ++first;
        }
        else {
            data_[i].~Base();
        }
    }
    size_ -= i - first; 
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::push_back(Base value) {
    expand_if_full();
    new (data_ + size_) Base{value};
    ++size_;
}


template<typename Base, typename Allocator> 
template<typename Derived, typename... Args>
requires emplaceable_from<Derived, Base>
void PolyVector<Base, Allocator>::emplace_back(Args&&... args) {
    expand_if_full();
    new (data_ + size_) Derived(std::forward<Args>(args)...);
    ++size_;
}

template<typename Base, typename Allocator> 
void PolyVector<Base, Allocator>::pop_back() {
    if (size_ > 0) {
        data_[size_ - 1].~Base();
        size_--;
    }
}

#endif
