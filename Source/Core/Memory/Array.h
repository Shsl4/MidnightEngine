#pragma once

#include <Core/EngineTypes.h>
#include <Memory/Allocator.h>
#include <type_traits>

/*!
 * A mutable array object that supports adding, inserting and removing objects.
 *
 * @tparam T The type of object to store
 */
template<typename T>
struct Array {

    explicit Array(size_t capacity = 15) {

        this->capacity = capacity;
        this->data = allocator.allocate<T>(capacity);
        memset(data, 0, capacity * sizeof(T));

    }

    Array(const Array<T> &other) {

        this->count = other.count;
        this->capacity = other.capacity;
        this->data = allocator.allocate<T>(capacity);
        memcpy(this->data, other.data, this->count * sizeof(T));

    }

    virtual ~Array() {

        count = 0;
        capacity = 0;
        allocator.deallocate(data);

    }

    /*!
     * Adds an element to the end of the array.
     *
     *  @tparam S The type of object to append
     *  @param[in] other The element to add
    */
    template<typename S>
    void append(S const &other) {

        if (count == capacity) {
            this->data = allocator.resize(data, count == 0 ? 15 : capacity * 2);
            capacity *= 2;
        }

        if (std::is_pointer_v<T>) {
            this->data[count] = other;
        } else {
            this->data[count] = S(other);
        }
        ++count;

    }

    /*!
     * Removes the input object from the array if it contains it.
     *
     *  @tparam S The type of object to remove
     *  @param[in] other The object to remove
     *  @return Whether the object was removed
     */
    template<typename S>
    bool remove(S const &other) {

        for (size_t i = 0; i < count; ++i) {

            if (other == data[i]) {

                for (size_t j = i; j < count - 1; ++j) {

                    data[j] = data[j + 1];

                }

                --count;
                memset(&data[count], 0, sizeof(T));
                return true;

            }

        }

        return false;

    }

    /*!
     * Checks whether the array contains a specific object.
     *
     *  @tparam S The type of object to check
     *  @param[in] other The object to check
     *  @return Whether the array contains other
     */
    template<typename S>
    bool contains(S const &other) const {

        for (size_t i = 0; i < count; ++i) {

            if (other == data[i]) {

                return true;

            }

        }

        return false;

    }

    /*!
     * Clears the array.
     *
     *  @warning If the array contains pointers, those will not be freed.
     */
    void clear() {

        memset(data, 0, capacity * sizeof(T));
        this->count = 0;

    }

    /*!
     * Shrinks the array buffer to the minimal size.
     */
    void shrink() {

        if (count == capacity) {return;}
        this->data = allocator.resize(data, count);
        this->capacity = count;

    }

    /*!
     * Resizes the array buffer to support a specific object count.
     *
     *  @param[in] size The number of objects the array can hold.
     *
     */
    void resize(size_t size) {

        this->data = allocator.resize(data, size);
        this->capacity = size;
        if (count > size) {count = size;}

    }

    /*!
     * Points to the beginning of the array buffer. This functions allows range based for-loops to be used, or to access the raw memory.
     */
    T *begin() const {

        return this->data;

    }

    /*!
     * Points to the end of the array buffer. This functions allows range based for-loops to be used.
     */
    T *end() const {

        return this->data + count;

    }

    /*!
     * Gets the element at the specified index.
     *
     *  @param[in] index The index of the object.
     */
    T operator[](size_t index) const {

        return *(data + index);

    }

    /*!
     * Shorter expression for append.
     *
     *  @tparam S The type of object to append
     *  @param[in] other The element to append
     */
    template<typename S>
    void operator+=(S const &other) {

        append(other);

    }

    FORCEINLINE bool isEmpty() const {
        return count == 0;
    }

    FORCEINLINE size_t getSize() const {
        return count;
    }

    FORCEINLINE size_t getCapacity() const {
        return capacity;
    }

private:

    T *data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Allocator allocator = Allocator();

};

/*!
 * Mutable Array type that auto-releases the contained objects upon destruction. T must be necessarily be a pointer.
 *
 *  @tparam T The type of object to store
 *  @warning Do not use this type as a return type as the object will be destroyed on function exit.
 */
template<typename T>
struct AutoReleaseArray : public Array<T> {

public:

    explicit AutoReleaseArray(size_t capacity = 15) : Array<T>(capacity) {

        static_assert(std::is_pointer_v<T>, "T must be a pointer.");

    }

    virtual ~AutoReleaseArray() {

        T *data = this->begin();

        for (size_t i = 0; i < this->getSize(); i++) {
            allocator.deallocate(data[i]);
        }

    }

private:

    Allocator allocator;

};
