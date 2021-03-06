#pragma once

#include <Memory/Allocator.h>
#include <Memory/RefCounter.h>

template<typename T>
class SharedPointer;

/*!
 * \brief A weak pointer class that references an object owned by a SharedPointer.
 */
template<typename T>
class WeakPointer final {

    template<typename Y>
    friend class WeakPointer;
        
    friend SharedPointer<T>;

public:

    WeakPointer() = default;
    
    ~WeakPointer();

    explicit WeakPointer(const SharedPointer<T>& other);

    WeakPointer(const WeakPointer& other);

    template<typename Y>
    WeakPointer(WeakPointer<Y> const& other);
    
    WeakPointer(nullptr_t);

    WeakPointer(WeakPointer&& other) noexcept;

    WeakPointer& operator=(const WeakPointer& other);

    WeakPointer& operator=(const SharedPointer<T>& other);

    WeakPointer& operator=(nullptr_t);

    WeakPointer& operator=(WeakPointer&& other) noexcept;

    template <typename Y>
    bool operator==(WeakPointer<Y> const& other) const noexcept;

    template <typename Y>
    bool operator!=(WeakPointer<Y> const& other) const noexcept;

    template <typename Y>
    bool operator<(WeakPointer<Y> const& other) const noexcept;

    NODISCARD SharedPointer<T> retain() const;

    FORCEINLINE bool expired() const;
    
    FORCEINLINE bool valid() const;

    T* operator->() const;

    NODISCARD T* raw() const;

private:

    void discard();

    T* pointer = nullptr;
    RefCounter* counter = nullptr;
    Allocator<RefCounter> refAllocator {};

};


#define __WEAK_PTR_INCL
#include <Memory/WeakPointer.inl>
#undef __WEAK_PTR_INCL
