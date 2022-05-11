#pragma once

#include <Memory/Allocator.h>
#include <Memory/RefCounter.h>
#include <Exception/Exception.h>

template <typename T>
class WeakPointer;

template <typename T>
class SharedPointer final : public Object
{

    template<typename Y>
    friend class SharedPointer;
    
    friend WeakPointer<T>;
    
    explicit SharedPointer(T* object);

public:

    template <typename ... Args>
    static SharedPointer<T> make(Args&&... args);

    SharedPointer() = default;

    ~SharedPointer() override;

    explicit SharedPointer(const WeakPointer<T>& weak);

    SharedPointer(const SharedPointer& other);

    template<typename Y>
    SharedPointer(SharedPointer<Y> const& other);

    SharedPointer(SharedPointer&& other) noexcept;

    SharedPointer& operator=(const SharedPointer& other);

    SharedPointer& operator=(SharedPointer&& other) noexcept;

    SharedPointer& operator=(nullptr_t);
    
    template <typename Y>
    bool operator==(SharedPointer<Y> const& other) noexcept;

    template <typename Y>
    bool operator!=(SharedPointer<Y> const& other) noexcept;

    template <typename Y>
    bool operator<(SharedPointer<Y> const& other) noexcept;

    NODISCARD WeakPointer<T> weak() const;

    T* operator->() const;

    NODISCARD T* raw() const;
    
    NODISCARD bool valid() const;

private:

    void discard();

    T* pointer = nullptr;
    RefCounter* counter = nullptr;
    Allocator<RefCounter> refAllocator {};
    Allocator<T> allocator {};
    
};

#define __SHARED_PTR_INCL
#include <Memory/SharedPointer.inl>
#undef __SHARED_PTR_INCL

