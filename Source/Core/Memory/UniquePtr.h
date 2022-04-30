#pragma once

#include <Memory/Allocator.h>
#include <Core/EngineTypes.h>

template<typename T>
class UniquePtr {

public:

    template<typename ... Args>
    static UniquePtr<T> make(Args &&... args) {

        return UniquePtr<T>(Allocator<T>().construct(std::forward<Args>(args)...));

    }

    UniquePtr() = default;

    UniquePtr<T>& operator=(UniquePtr<T> const& other) = delete;

    UniquePtr<T>& operator=(nullptr_t) {
        allocator.destroy(pointer);
        return *this;
    };

    UniquePtr<T>& operator=(UniquePtr<T>&& other) {

        if (pointer)
        {
            allocator.destroy(pointer);
        }

        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;

    }

    ~UniquePtr() {
        allocator.destroy(pointer);
    }

    T* operator->() const noexcept {
        return pointer;
    }

    bool operator!() const noexcept {
        return !pointer;
    }

    FORCEINLINE T* get() const {
        return pointer;
    }

    FORCEINLINE bool hasValue() {
        return pointer;
    }

private:

    UniquePtr(T* newPointer) {
        pointer = newPointer;
    }

    T* pointer = nullptr;
    Allocator<T> allocator;

};