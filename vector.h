#include <cstddef>
#include <memory>
#include <iostream>


template<typename Base, typename Allocator = std::allocator<Base>> 
class PolyVector {
public:
    // Member functions
    ~PolyVector();

    // Element access
    Base& operator[](size_t index);
    Base& front();
    Base& back();
    Base* data();

    // Capacity
    size_t size();
    void reserve(size_t new_capacity);

    // Modifiers
    void push_back(Base value);
    

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
