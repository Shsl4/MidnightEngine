#pragma once

#ifndef __ALLOC_INCL
#error This file sould only be included from Allocator.h
#endif

template<typename T>
template<typename ... Args>
T *Allocator<T>::construct(Args &&... args) {

    // Construct the new object.
    T *ptr = new T(std::forward<Args>(args)...);

    // Return the newly constructed object
    return ptr;

}

template<typename T>
T *Allocator<T>::alloc(size_t size) {

    // If the size is 0, do not allocate anything.
    if (size <= 0) {
        return nullptr;
    }

    // Allocate the new array;
    T *ptr = new T[size];

    // Fill the buffer with zeroes if T is a primitive type.
    if (std::is_fundamental_v<T>) {
        std::memset(ptr, 0, size);
    }

    // Return the newly allocated buffer.
    return ptr;
}

template<typename T>
void Allocator<T>::destroy(T *&pointer) {

    // If the pointer is empty, return.
    if (!pointer) {
        return;
    }

    // Release the pointer.
    delete pointer;

    // Set the input variable to nullptr.
    pointer = nullptr;

}

template<typename T>
template<typename U>
void Allocator<T>::autoDestroy(U *&pointer) {

    // If the pointer is empty, return.
    if (!pointer) {
        return;
    }

    // Release the pointer.
    delete pointer;

}

template<typename T>
void Allocator<T>::release(T *&pointer) {

    // If the pointer is empty, return.
    if (!pointer) {
        return;
    }

    // Release the pointer.
    delete[] pointer;

    // Set the input variable to nullptr.
    pointer = nullptr;

}

template<typename T>
void Allocator<T>::realloc(T *&pointer, size_t oldSize, size_t size) {

    // If the size is 0,
    if (size == 0) {

        // Release the pointer and return.
        release(pointer);
        return;
    }

    // If the pointer is empty,
    if (!pointer) {

        // Allocate the buffer.
        pointer = alloc(size);
        return;

    }

    // Allocate a new buffer.
    T *p = alloc(size);

    // If the new size is lower than the old size, move objects up to this new size.
    size_t sz = size > oldSize ? oldSize : size;

    // Move back the objects.
    Memory::moveBack(pointer, pointer + sz, p);

    // Release the old buffer.
    release(pointer);

    // Set the input variable to the new buffer.
    pointer = p;

}

template<typename T>
void Allocator<T>::move(T *from, T *to, T *dest) {
    Memory::move(from, to, dest);
}

template<typename T>
void Allocator<T>::copy(const T *from, const T *to, T *dest) {
    Memory::copy(from, to, dest);
}

template<typename T>
void Allocator<T>::swapMove(T *a, T *b) {
    Memory::swapMove(a, b);
}