#pragma once

#include <Core/Object.h>
#include <Memory/Allocator.h>
#include <Memory/RefCounter.h>

template<typename T>
class SharedPointer;

template<typename T>
class WeakPointer final : public Object {

    template<typename Y>
    friend class WeakPointer;
        
    friend SharedPointer<T>;

public:

    WeakPointer() = default;
    
    ~WeakPointer() override;

    explicit WeakPointer(const SharedPointer<T>& other);

    WeakPointer(const WeakPointer& other);
    
    WeakPointer(nullptr_t);

    WeakPointer(WeakPointer&& other) noexcept;

    WeakPointer& operator=(const WeakPointer& other);

    WeakPointer& operator=(const SharedPointer<T>& other);

    WeakPointer& operator=(nullptr_t);

    WeakPointer& operator=(WeakPointer&& other) noexcept;

    template <typename Y>
    bool operator==(WeakPointer<Y> const& other) noexcept;

    template <typename Y>
    bool operator!=(WeakPointer<Y> const& other) noexcept;

    template <typename Y>
    bool operator<(WeakPointer<Y> const& other) noexcept;

    NODISCARD SharedPointer<T> retain() const;

    FORCEINLINE bool expired();
    
    FORCEINLINE bool valid();

    T* operator->();

    NODISCARD T* raw();

private:

    void discard();

    T* pointer = nullptr;
    RefCounter* counter = nullptr;
    Allocator<RefCounter> refAllocator {};

};


#define __WEAK_PTR_INCL
#include <Memory/WeakPointer.inl>
#undef __WEAK_PTR_INCL
