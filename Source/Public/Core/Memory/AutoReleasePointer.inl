#pragma once

#ifndef __ARP_INCL
#error This file sould only be included from AutoReleasePointer.h
#endif

template <typename T>
template <typename ... Args>
AutoReleasePointer<T> AutoReleasePointer<T>::make(Args&&... args)
{
    return AutoReleasePointer<T>(Allocator<T>().construct(std::forward<Args>(args)...));
}

template <typename T>
AutoReleasePointer<T> AutoReleasePointer<T>::empty()
{
    return AutoReleasePointer<T>();
}

template <typename T>
AutoReleasePointer<T>::AutoReleasePointer(AutoReleasePointer<T>&& other) noexcept
{
    release();
    pointer = other.pointer;
    other.pointer = nullptr;
}

template <typename T>
void AutoReleasePointer<T>::release()
{
    allocator.destroy(pointer);
}

template <typename T>
void AutoReleasePointer<T>::discard()
{
    pointer = nullptr;
}

template <typename T>
template <typename OtherType>
void AutoReleasePointer<T>::manage(OtherType* ptr)
{
    static_assert(std::is_convertible_v<OtherType*, T*>, "OtherType must publicly inherit T");

    release();
    pointer = ptr;
}

template <typename T>
template <typename OtherType, typename ... Args>
void AutoReleasePointer<T>::rebuild(Args&&... args)
{
    static_assert(std::is_convertible_v<OtherType*, T*>, "OtherType must publicly inherit T");

    release();
    pointer = Allocator<OtherType>().construct(std::forward<Args>(args)...);
}

template <typename T>
AutoReleasePointer<T>& AutoReleasePointer<T>::operator=(nullptr_t)
{
    release();
    return *this;
}

template <typename T>
AutoReleasePointer<T>& AutoReleasePointer<T>::operator=(AutoReleasePointer<T>&& other) noexcept
{
    release();
    pointer = other.pointer;
    other.pointer = nullptr;
    return *this;
}

template <typename T>
AutoReleasePointer<T>::~AutoReleasePointer()
{
    release();
}

template <typename T>
T* AutoReleasePointer<T>::operator->() const noexcept
{
    return pointer;
}

template <typename T>
T* AutoReleasePointer<T>::raw() const
{
    return pointer;
}

template <typename T>
bool AutoReleasePointer<T>::hasValue() const
{
    return pointer;
}

template <typename T>
AutoReleasePointer<T>::AutoReleasePointer(T* newPointer)
{
    pointer = newPointer;
}
