#pragma once

#ifndef __ARP_INCL
#error This file sould only be included from AutoReleasePointer.h
#endif

template <typename T>
template <typename ... Args>
UniquePointer<T> UniquePointer<T>::make(Args&&... args) {
    return UniquePointer<T>(Allocator<T>().construct(std::forward<Args>(args)...));
}

template <typename T>
UniquePointer<T> UniquePointer<T>::empty() { return UniquePointer<T>(); }

template <typename T>
UniquePointer<T>::UniquePointer(UniquePointer<T>&& other) noexcept {
    release();
    pointer = other.pointer;
    other.pointer = nullptr;
}

template <typename T>
void UniquePointer<T>::release() { allocator.destroy(pointer); }

template <typename T>
void UniquePointer<T>::discard() { pointer = nullptr; }

template <typename T>
template <typename OtherType>
void UniquePointer<T>::manage(OtherType* ptr) {
    static_assert(std::is_convertible_v<OtherType*, T*>, "OtherType must publicly inherit T");

    release();
    pointer = ptr;
}

template <typename T>
template <typename OtherType, typename ... Args>
void UniquePointer<T>::rebuild(Args&&... args) {
    static_assert(std::is_convertible_v<OtherType*, T*>, "OtherType must publicly inherit T");

    release();
    pointer = Allocator<OtherType>().construct(std::forward<Args>(args)...);
}

template <typename T>
UniquePointer<T>& UniquePointer<T>::operator=(nullptr_t) {
    release();
    return *this;
}

template <typename T>
UniquePointer<T>& UniquePointer<T>::operator=(UniquePointer<T>&& other) noexcept {
    release();
    pointer = other.pointer;
    other.pointer = nullptr;
    return *this;
}

template <typename T>
UniquePointer<T>::~UniquePointer() { release(); }

template <typename T>
T* UniquePointer<T>::operator->() const {
    expect(pointer, "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
T* UniquePointer<T>::raw() const {
    expect(pointer, "Tried to dereference a null pointer.");
    return pointer;
}

template <typename T>
bool UniquePointer<T>::hasValue() const { return pointer; }

template <typename T>
UniquePointer<T>::UniquePointer(T* newPointer) { pointer = newPointer; }
