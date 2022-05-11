#pragma once

#ifndef __SHARED_PTR_INCL
#error This file sould only be included from SharedPointer.h
#endif

#include <Memory/WeakPointer.h>

template <typename T>
SharedPointer<T>::SharedPointer(T* object) {

    this->counter = refAllocator.construct();
    this->pointer = object;

    counter->increase();

}

template <typename T>
template <typename ... Args>
SharedPointer<T> SharedPointer<T>::make(Args&&... args) {
    return SharedPointer<T>(Allocator<T>().construct(std::forward<Args>(args)...));
}

template <typename T>
SharedPointer<T>::~SharedPointer() { discard(); }

template <typename T>
SharedPointer<T>::SharedPointer(const WeakPointer<T>& weak) {
    
    if (!weak.expired()) {
        
        this->counter = weak.pointer;
        this->pointer = weak.counter;

        if (pointer) { counter->increase(); }
        
    }
    
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer& other) {
    
    if (other.pointer) {
        
        this->pointer = other.pointer;
        this->counter = other.counter;

        counter->increase();
    }
    
}

template <typename T>
template<typename Y>
SharedPointer<T>::SharedPointer(SharedPointer<Y> const& other) {

    static_assert(std::is_convertible_v<Y*, T*>);
    
    if (other.pointer) {
        
        this->pointer = other.pointer;
        this->counter = other.counter;

        counter->increase();
    }
    
}

template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer&& other) noexcept {

    this->pointer = other.pointer;
    this->counter = other.counter;

    other.pointer = nullptr;
    other.counter = nullptr;

}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer& other) {

    if (this == &other)
        return *this;

    discard();

    if (other.pointer) {
        this->pointer = other.pointer;
        this->counter = other.counter;

        counter->increase();
    }

    return *this;
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer&& other) noexcept {

    if (this == &other)
        return *this;

    discard();

    this->pointer = other.pointer;
    this->counter = other.counter;

    other.pointer = nullptr;
    other.counter = nullptr;

    return *this;

}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(nullptr_t) {
    discard();
    return *this;
}

template <typename T>
template <typename Y>
bool SharedPointer<T>::operator==(SharedPointer<Y> const& other) noexcept {
    return pointer == other.pointer;
}

template <typename T>
template <typename Y>
bool SharedPointer<T>::operator!=(SharedPointer<Y> const& other) noexcept {
    return pointer != other.pointer;
}

template <typename T>
template <typename Y>
bool SharedPointer<T>::operator<(SharedPointer<Y> const& other) noexcept {
    return pointer < other.pointer;
}

template <typename T>
WeakPointer<T> SharedPointer<T>::weak() const { return WeakPointer<T>(*this); }

template <typename T>
T* SharedPointer<T>::operator->() const {
    expect(pointer, "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
T* SharedPointer<T>::raw() const {
    expect(pointer, "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
bool SharedPointer<T>::valid() const {
    return pointer;
}

template <typename T>
void SharedPointer<T>::discard() {

    if (!pointer) { return; }

    counter->decrease();

    if (counter->exhausted()) {
        if (counter->exhaustedWeak()) { refAllocator.destroy(counter); }

        allocator.destroy(pointer);
    }

    counter = nullptr;
    pointer = nullptr;

}
