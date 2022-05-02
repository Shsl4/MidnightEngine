#pragma once

#include <Memory/Allocator.h>
#include <Core/EngineTypes.h>

template<typename T>
class ARPointer {

public:

    template<typename ... Args>
    static ARPointer<T> make(Args &&... args) {

        return ARPointer<T>(Allocator<T>().construct(std::forward<Args>(args)...));

    }

    ARPointer() = default;

    ARPointer(const ARPointer<T>& other) = delete;
    
    ARPointer(ARPointer<T>&& other) noexcept { 
        
        release();
        pointer = other.pointer;
        other.pointer = nullptr;

     }

    ARPointer<T>& operator=(ARPointer<T> const& other) = delete;

    void release() {
        allocator.destroy(pointer);
    }

    template<typename OtherType>
    void manage(T* ptr) {

        static_assert(std::is_convertible<OtherType*, T*>::value, "OtherType must publicly inherit T");

        release();
        pointer = ptr;

    }

    template<typename OtherType, typename ... Args>
    void rebuild(Args &&... args) {

        static_assert(std::is_convertible<OtherType*, T*>::value, "OtherType must publicly inherit T");

        release();
        pointer = Allocator<OtherType>().construct(std::forward<Args>(args)...);

    }

    ARPointer<T>& operator=(nullptr_t) {
        release();
        return *this;
    }

    ARPointer<T>& operator=(ARPointer<T>&& other) noexcept {

        release();
        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;

    }

    ~ARPointer() {
        release();
    }

    T* operator->() const noexcept {
        return pointer;
    }

    bool operator!() const noexcept {
        return !pointer;
    }

    FORCEINLINE T* raw() const {
        return pointer;
    }

    FORCEINLINE bool hasValue() {
        return pointer;
    }

private:

    ARPointer(T* newPointer) {
        pointer = newPointer;
    }

    T* pointer = nullptr;
    Allocator<T> allocator;

};