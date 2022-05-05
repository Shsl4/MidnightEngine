#pragma once

#include <Memory/Allocator.h>

/**
 * \brief A smart unique pointer type that auto-releases upon destruction.
 * \tparam T The type of object to manage
 */
template <typename T>
class AutoReleasePointer : public Object
{
    /**
     * \brief The AutoReleasePointer constructor used internally to manage an object
     * \param newPointer The pointer to manage.
     */
    explicit AutoReleasePointer(T* newPointer);

public:
    /**
     * \brief Creates a new AutoReleasePointer of type T
     * \tparam Args A variadic template type
     * \param args The arguments passed to the object constructor.
     * \return The new pointer
     */
    template <typename ... Args>
    static AutoReleasePointer<T> make(Args&&... args);

    /**
     * \brief Creates an empty AutoReleasePointer.
     * \return The new pointer object
     */
    static AutoReleasePointer<T> empty();

    /**
    * \brief The default AutoReleasePointer constructor
    */
    AutoReleasePointer() = default;

    /**
     * \brief The AutoReleasePointer destructor. Releases the managed resource.
     */
    ~AutoReleasePointer();

    /**
     * \brief The AutoReleasePointer copy constructor (deleted)
     * \param other The AutoReleasePointer to copy
     */
    AutoReleasePointer(const AutoReleasePointer<T>& other) = delete;

    /**
     * \brief The AutoReleasePointer move constructor
     * \param other The pointer to move
     */
    AutoReleasePointer(AutoReleasePointer<T>&& other) noexcept;

    /**
     * \brief The AutoReleasePointer copy assignment operator (deleted)
     * \param other The AutoReleasePointer to copy
     */
    AutoReleasePointer<T>& operator=(AutoReleasePointer<T> const& other) = delete;

    /**
     * \brief The nullptr assignment operator. Releases the managed resource.
     * \return This empty pointer
     */
    AutoReleasePointer<T>& operator=(nullptr_t);

    /**
     * \brief The AutoReleasePointer move constructor
     * \param other The pointer to move
     * \return This pointer
     */
    AutoReleasePointer<T>& operator=(AutoReleasePointer<T>&& other) noexcept;

    /**
     * \brief Releases the managed resource.
     */
    void release();

    /**
     * \brief Stops managing the resource without releasing it.
     */
    void discard();

    /**
     * \brief Manages the input raw pointer. Releases the current resource if allocated.
     * \tparam OtherType The type of object to manage
     * \param ptr The pointer to manage
     */
    template <typename OtherType>
    void manage(OtherType* ptr);

    /**
     * \brief Releases the managed resource and reconstructs a new object of type OtherType.
     * \tparam OtherType The type of object to construct. Must inherit or be T.
     * \tparam Args A variadic template type
     * \param args The arguments passed to the object constructor.
     */
    template <typename OtherType, typename ... Args>
    void rebuild(Args&&... args);

    /**
     * \brief Returns the raw pointer managed by this object
     * \return The raw pointer
     */
    T* operator->() const noexcept;

    /**
     * \brief Returns the raw pointer managed by this object
     * \return The raw pointer
     */
    FORCEINLINE T* raw() const;

    /**
     * \brief Checks whether this pointer is allocated.
     * \return Whether this pointer has a value
     */
    FORCEINLINE bool hasValue() const;

private:
    /**
     * \brief The managed resource.
     */
    T* pointer = nullptr;

    /**
     * \brief The allocator used to manage memory for this pointer.
     */
    Allocator<T> allocator;
};

#define __ARP_INCL

#include <Memory/AutoReleasePointer.inl>

#undef __ARP_INCL
