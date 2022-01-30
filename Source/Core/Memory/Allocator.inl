#pragma once

#ifndef __ALLOC_INCL
#error This file sould only be included from Allocator.h
#endif

template<typename T>
Allocator<T>::Allocator() {

#ifdef DEBUG
    // Increase the number of created allocators.
    ++DebugAlloc::info.constructCount;
#endif

}

template<typename T>
template<typename ... Args>
T *Allocator<T>::construct(Args &&... args) {

#ifdef DEBUG
    // Increase the number of total allocations.
    ++DebugAlloc::info.allocCount;
#endif

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

#ifdef DEBUG
    // Increase the number of total allocations.
    ++DebugAlloc::info.allocCount;
#endif

    // Return the newly allocated buffer.
    return ptr;
}

template<typename T>
void Allocator<T>::destroy(T *&pointer) {

    // If the pointer is empty, return.
    if (!pointer) {
        return;
    }

#ifdef DEBUG
    // Increase the number of total deallocations.
    ++DebugAlloc::info.releaseCount;
#endif

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

#ifdef DEBUG
    // Increase the number of total deallocations.
    ++DebugAlloc::info.releaseCount;
#endif

    // Release the pointer.
    delete pointer;

}

template<typename T>
void Allocator<T>::release(T *&pointer) {

    // If the pointer is empty, return.
    if (!pointer) {
        return;
    }

#ifdef DEBUG
    // Increase the number of total deallocations.
    ++DebugAlloc::info.releaseCount;
#endif

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
    moveBack(pointer, pointer + sz, p);

    // Release the old buffer.
    release(pointer);

    // Set the input variable to the new buffer.
    pointer = p;

#ifdef DEBUG
    // Increase the total number of reallocations.
    ++DebugAlloc::info.reallocCount;
#endif

}

template<typename T>
Allocator<T>::~Allocator() {

#ifdef DEBUG
    // Decrease the global amount of allocator instances.
    --DebugAlloc::info.constructCount;
#endif
    
}

template<typename T>
void Allocator<T>::move(T *from, T *to, T *dest) {

    if (dest > from) {
        moveForward(from, to, dest);
        return;
    }

    moveBack(from, to, dest);

}

template<typename T>
void Allocator<T>::copy(const T *from, const T *to, T *dest) {

    if (dest > from) {
        copyForward(from, to, dest);
        return;
    }

    copyBack(from, to, dest);

}

template<typename T>
void Allocator<T>::swapMove(T *a, T *b) {

    T temp = std::move(*a);
    *a = std::move(*b);
    *b = std::move(temp);

}

template<typename T>
void Allocator<T>::moveForward(T *from, T *to, T *dest) {

    dest += to - from - 1;

    while (from != to) {
        *dest = std::move(*(to - 1));
        --dest;
        --to;
    }

}

template<typename T>
void Allocator<T>::moveBack(T *from, T *to, T *dest) {

    while (from != to) {

        *dest = std::move(*from);
        ++dest;
        ++from;

    }

}

template<typename T>
void Allocator<T>::copyForward(const T *from, const T *to, T *dest) {

    dest += to - from - 1;

    while (from != to) {
        *dest = *(to - 1);
        --dest;
        --to;
    }

}

template<typename T>
FORCEINLINE void Allocator<T>::copyBack(const T *from, const T *to, T *dest) {

    while (from != to) {

        *dest = *from;
        ++dest;
        ++from;

    }

}
