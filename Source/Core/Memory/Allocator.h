#pragma once

#include <stdlib.h>

/*!
 * Object used to dynamically allocate memory. Allocators can allocate memory buffers,
 * deallocate and resize them as well as instantiate new objects.
 */
class Allocator {

public:

    Allocator() = default;

    /*!
     * Resizes a memory buffer. If the input size is zero, the pointer is freed.
     *
     *  @tparam T The type of objects to hold in the buffer
     *  @param[in] ptr The buffer to resize.
     *  @param[in] count The number of objects to hold
     *  @return The new buffer
     */
    template<typename T>
    T *resize(T *ptr, const size_t count) {

        return static_cast<T *>(realloc(ptr, sizeof(T) * count));

    }

    /*!
     * Duplicates the input object.
     *
     *  @tparam T The type to copy
     *  @param[in] other The object to duplicate
     *  @return The newly created object
     */
    template<typename T>
    T *copy(T *&other) {
        return new T(other);
    }

    /*!
     * Instantiates a nww object of class T using the arguments as constructor parameters.
     *
     *  @tparam T The type of object to instantiate
     *  @tparam Args A variadic arguments template
     *  @param[in] args The arguments to pass to the constructor
     *  @return The newly created object.
     */
    template<class T, class... Args>
    T *instantiate(Args &&... args) {
        return new T(std::forward<Args>(args)...);
    }

    /*!
     * Allocates a new memory buffer WITHOUT constructing objects.
     *
     *  @tparam T The type of object to hold
     *  @param[in] count The number of objects to hold
     *  @return The allocated memory buffer.
     */
    template<typename T>
    T *allocate(size_t count) const {

        return static_cast<T *>(operator new(sizeof(T) * count));

    }

    /*!
     *  Deallocates the input buffer and sets the reference to nullptr.
     *
     *  @tparam T The type of object to deallocate
     *  @param[in] pointer The memory to deallocate
     */
    template<typename T>
    void deallocate(T *&pointer) {

        if (!pointer) return;
        delete pointer;
        pointer = nullptr;

    }

};
