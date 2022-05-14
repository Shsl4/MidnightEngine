#pragma once

#ifndef __WEAK_PTR_INCL
#error This file sould only be included from WeakPointer.h
#endif

#include <Memory/SharedPointer.h>

template <typename T>
WeakPointer<T>::~WeakPointer()
{
    discard();
}

template <typename T>
WeakPointer<T>::WeakPointer(const SharedPointer<T>& other) : pointer(other.pointer), counter(other.counter)
{

    if (pointer) {
        counter->increaseWeak();
    }

}

template <typename T>
WeakPointer<T>::WeakPointer(const WeakPointer& other) : pointer(other.pointer), counter(other.counter)
{

    if (pointer) {
        counter->increaseWeak();
    }

}

template <typename T>
template <typename Y>
WeakPointer<T>::WeakPointer(WeakPointer<Y> const& other) {
    
    static_assert(std::is_base_of_v<Y, T>);
    
    if (other.pointer) {
        
        this->pointer = dynamic_cast<T*>(other.pointer);
        this->counter = other.counter;

        counter->increaseWeak();
    }
    
}

template <typename T>
WeakPointer<T>::WeakPointer(nullptr_t) {}

template <typename T>
WeakPointer<T>::WeakPointer(WeakPointer&& other) noexcept : pointer(other.pointer), counter(other.counter)
{

    other.pointer = nullptr;
    other.counter = nullptr;

}

template <typename T>
WeakPointer<T>& WeakPointer<T>::operator=(const WeakPointer& other)
{
    if (this == &other)
        return *this;

    discard();

    pointer = other.pointer;
    counter = other.counter;

    if (pointer) {
        counter->increaseWeak();
    }

    return *this;
}

template <typename T>
WeakPointer<T>& WeakPointer<T>::operator=(const SharedPointer<T>& other)
{

    discard();

    pointer = other.pointer;
    counter = other.counter;

    if (pointer) {
        counter->increaseWeak();
    }

    return *this;
}

template <typename T>
WeakPointer<T>& WeakPointer<T>::operator=(nullptr_t)
{
    discard();
    return *this;
}

template <typename T>
WeakPointer<T>& WeakPointer<T>::operator=(WeakPointer&& other) noexcept
{
    if (this == &other)
        return *this;

    discard();

    pointer = other.pointer;
    counter = other.counter;

    other.pointer = nullptr;
    other.counter = nullptr;

    return *this;
}

template <typename T>
template <typename Y>
bool WeakPointer<T>::operator==(WeakPointer<Y> const& other) noexcept {
    return pointer == other.pointer;
}

template <typename T>
template <typename Y>
bool WeakPointer<T>::operator!=(WeakPointer<Y> const& other) noexcept {
    return pointer != other.pointer;
}

template <typename T>
template <typename Y>
bool WeakPointer<T>::operator<(WeakPointer<Y> const& other) noexcept {
    return pointer < other.pointer;
}

template <typename T>
SharedPointer<T> WeakPointer<T>::retain() const
{
    return SharedPointer<T>(*this);
}

template <typename T>
bool WeakPointer<T>::expired()
{

    if (!counter) {
        return true;
    }

    if (counter->exhausted()) {
        discard();
        return true;
    }

    return false;

}

template <typename T>
bool WeakPointer<T>::valid() {
    return !expired();
}

template <typename T>
T* WeakPointer<T>::operator->()
{
    expect(valid(), "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
T* WeakPointer<T>::raw() {
    expect(valid(), "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
void WeakPointer<T>::discard()
{

    if (!pointer) { return; }

    counter->decreaseWeak();

    if (counter->exhausted() && counter->exhaustedWeak()) {
        refAllocator.destroy(counter);
    }

    counter = nullptr;
    pointer = nullptr;

}
