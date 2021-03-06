#pragma once

#include <functional>

#include <Core/EngineTypes.h>
#include <Memory/Memory.h>
#include <Memory/Allocator.h>
#include <Utilities/Optional.h>

/*!
 *  \brief A mutable array object that supports adding, inserting, removing, sorting and other utility functions.
 *
 *  \tparam T The type of element to store
 *  \warning T must implement an equality and lower-than operator.
 */
template <typename T>
class Array {

public:
    /*!
     *  The default array constructor.
     */
    Array() = default;

    /*!
     *  Constructs an array with a specific pre-allocated capacity.
     *
     *  \param[in] capacity The number of elements to allocate
     */
    explicit Array(size_t capacity);

    /*!
     *  The Array copy constructor.
     *
     *  \param[in] other The array to copy
     */
    Array(Array<T> const& other);

    /*!
     *  The Array copy constructor with a size limit.
     *
     *  \param[in] other The array to copy
     *  \param[in] limit The max number of elements to copy.
     */
    Array(Array<T> const& other, size_t limit);

    /*!
     *  The Array move constructor.
     *
     *  \param[in] other The array to move
     */
    Array(Array<T>&& other) noexcept;

    /*!
     * The Array destructor. It releases the allocated resources.
     */
    virtual ~Array();

    /*!
     *  Adds an element to the end of the array.
     *
     *  \param[in] elem The element to add
    */
    virtual T& append(T const& elem);

    /*!
     *  Adds the content of the input array to the end of this array.
     *
     *  \param[in] arr The element to add
    */
    virtual void append(Array<T> const& arr);

    /*!
     *  Removes the element at the index.
     *
     *  \param[in] index The index of the element to remove
     *  \return An optional value containing the element if it was removed.
     *
     *  \warning If T is a pointer type, removing an element from the array does not release it.
    */
    virtual Optional<T> removeAt(size_t index);

    /*!
     *  Returns an array containing the elements in range.
     *
     *  \param[in] from The index to start copying.
     *  \param[in] to The index to stop copying.
     *  \return An array containing every element within the range.
     */
    virtual Array<T> arrayWithRange(size_t from, size_t to);

    /*!
     *  Returns an array containing the elements matching the input condition.
     *
     *  \param[in] condition The condition to check
     *  \return An array containing all elements matching the condition.
     */
    virtual Array<T> arrayMatching(std::function<bool(T const &)> condition) const;

 /*!
     *  Inserts an element at the input index.
     *
     *  \param[in] elem The element to insert
     *  \param[in] index The position of the element to insert.
     */
    virtual void insert(T const& elem, size_t index);

    /*!
     *  Inserts the contents of an array at the input index.
     *
     *  \param[in] arr The array to insert
     *  \param[in] index The position of the element to insert.
     */
    virtual void insert(Array<T> const& arr, size_t index);

    /*!
     * Resizes the array buffer to support a specific element count.
     *
     *  \param[in] newCap The number of elements the array can hold.
     *  \warning Resizing the array can leak memory if T is a pointer type.
     *
     */
    virtual void resize(size_t newCap);

    /*!
     * Shrinks the array buffer to the minimal size.
     */
    virtual void shrink();

    /*!
     *  Clears and releases the array.
     *
     *  \warning If the array contains pointers, clearing the array does not release them.
     */
    virtual void clear();

    /*!
     * Reverses the array.
     */
    virtual void reverse();

    /*!
     *  Returns the element at the input index if it exists
     *
     *  \param[in] index The index of the element to get.
     *  \return The element at the input index.
     *  \throws An std::runtime_error if the index is out of bounds.
     */
    NODISCARD virtual T& getAt(size_t index) const;

    /*!
     * Gets the element at the specified index.
     *
     *  \param[in] index The index of the element.
     */
    virtual T& operator[](size_t index) const;

    /*!
     *  The Array copy assignment operator.
     *
     *  \param[in] other The array to copy.
     *  \return This array.
     */
    virtual Array<T>& operator=(Array<T> const& other);

    /*!
     *  The Array move assignment operator.
     *
     *  \param[in] other The array to move.
     *  \return This array.
     */
    virtual Array<T>& operator=(Array<T>&& other) noexcept;
 
    /*!
     *  Shorthand expression for append.
     *
     *  \param[in] elem The element to append
     */
    virtual void operator+=(T const& elem);

    /*!
     *  Shorthand expression for append.
     *
     *  \param[in] arr The array to append
     */
    virtual void operator+=(Array<T> const& arr);

    /*!
     * Points to the beginning of the array buffer. This functions allows range based for-loops to be used.
     */
    NODISCARD T* begin() const;

    /*!
     * Points to the end of the array buffer. This functions allows range based for-loops to be used.
     */
    NODISCARD T* end() const;

    /*!
     * Returns whether the array is empty.
     *
     * \return Whether the array is empty.
     */
    FORCEINLINE virtual bool isEmpty() const { return size == 0; }

    /*!
     * Returns the number of elements contained in the array.
     *
     * \return The number of elements in the array.
     */
    FORCEINLINE virtual size_t getSize() const { return this->size; }

    /*!
     * Returns the number of elements the array can hold without resizing the data buffer.
     *
     * \return The array capacity.
     */
    FORCEINLINE virtual size_t getCapacity() const { return this->capacity; }

protected:
    /*!
     *  Function called when adding an element. It extends the data buffer the array is full.
     *
     *  \param[in] ext The extension size.
     */
    virtual void extend(size_t ext = 1);

    /*!
     *  The data contained in the array.
     */
    T* data = nullptr;

    /*!
     * The number of elements in the array.
     */
    size_t size = 0;

    /*!
     * The size of the data buffer.
     */
    size_t capacity = 0;

    /*!
     * The allocator used to allocate and resize buffers.
     */
    Allocator<T> allocator{};

private:

    friend class String;

};

#define __ARRAY_INCL

#include <Memory/Array.inl>

#undef __ARRAY_INCL
