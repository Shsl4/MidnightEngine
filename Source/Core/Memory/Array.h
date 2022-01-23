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

    /*!
     * The default array constructor. Holds 10 object by default.
     */
    explicit Array(size_t capacity = 10) : capacity(capacity) {

        // Allocate our data.
        this->data = allocator.allocate<T>(capacity);
        
        // Fill our buffer with zeroes.
        memset(data, 0, capacity * sizeof(T));

    }

    /*!
     * The Array copy constructor.
     *
     * @param[in] other The array to copy
     */
    Array(const Array<T> &other) {

        // Copy the variables.
        this->count = other.count;
        this->capacity = other.capacity;
        
        // Allocate our data.
        this->data = allocator.allocate<T>(capacity);
        
        // Copy the data from the other array into this array.
        memcpy(this->data, other.data, this->count * sizeof(T));

    }

    /*!
     * The Array destructor. It releases the allocated resources.
     */
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

        // If the array is full
        if (count == capacity) {
            
            // If the capacity is 0, set it to 10, otherwise, double the array capacity.
            size_t newCapacity = capacity == 0 ? 10 : capacity * 2;
            
            // Resize our data buffer.
            this->data = allocator.resize(data, newCapacity);
            capacity = newCapacity;
            
        }

        // If T is a pointer type
        if (std::is_pointer_v<T>) {
            
            // Directly store the pointer.
            this->data[count] = other;
            
        } else {
            
            // Else, copy the object as the argument is passed by reference.
            this->data[count] = S(other);
            
        }
        
        // Increase the number of held objects.
        ++count;

    }

    /*!
     * Removes the input object from the array if it contains it.
     *
     *  @tparam S The type of object to remove
     *  @param[in] other The object to remove
     *  @return Whether the object was removed
     *
     *  @warning If T is a pointer type, removing an element from the array does not release it.
     */
    template<typename S>
    bool remove(S const &other) {

        // For each element in the array
        for (size_t i = 0; i < count; ++i) {

            // If other was found if the array
            if (other == data[i]) {

                // Move back all the elements after
                for (size_t j = i; j < count - 1; ++j) {

                    data[j] = data[j + 1];

                }

                // Reduce the count.
                --count;
                
                // Fill the last moved element with zeroes.
                memset(&data[count], 0, sizeof(T));
                return true;

            }

        }

        // The element wasn't found in the array
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

        // For each element in the array
        for (size_t i = 0; i < count; ++i) {

            // If other was found
            if (other == data[i]) {

                return true;

            }

        }

        // The element wasn't found in the array.
        return false;

    }
    
    /*!
     * Checks whether the array contains a specific object and returns the index.
     *
     *  @tparam S The type of object to check
     *  @param[in] other The object to check
     *  @param[out] index The index of the element in the array.
     *  @return Whether the array contains other
     */
    template<typename S>
    bool contains(S const &other, size_t &index) const {

        // For each element in the array
        for (size_t i = 0; i < count; ++i) {

            // If other was found
            if (other == data[i]) {

                // Set the index and return true.
                index = i;
                return true;

            }

        }

        // The element wasn't found in the array.
        return false;

    }

    /*!
     * Clears the array.
     *
     *  @warning If the array contains pointers, clearing the array does not release them.
     */
    virtual void clear() {

        // Fill the array with zeroes.
        memset(data, 0, capacity * sizeof(T));
        
        // Empty the count.
        this->count = 0;

    }

    /*!
     * Shrinks the array buffer to the minimal size.
     */
    virtual void shrink() {

        // If the array can't be shrunk, return.
        if (count == capacity) { return; }
        
        // Shrink the array to the number of elements contained.
        this->data = allocator.resize(data, count);
        this->capacity = count;

    }

    /*!
     * Resizes the array buffer to support a specific object count.
     *
     *  @param[in] size The number of objects the array can hold.
     *  @warning Resizing the array can leak memory if T is a pointer type.
     *
     */
    virtual void resize(size_t size) {

        // Resize the data buffer.
        this->data = allocator.resize(data, size);
        this->capacity = size;
        if (count > size) { count = size; }

    }

    /*!
     * Points to the beginning of the array buffer. This functions allows range based for-loops to be used, or to access the raw memory.
     */
    virtual T *begin() const {

        return this->data;

    }

    /*!
     * Points to the end of the array buffer. This functions allows range based for-loops to be used.
     */
    virtual T *end() const {

        return this->data + count;

    }

    /*!
     * Gets the element at the specified index.
     *
     *  @param[in] index The index of the object.
     */
    virtual T operator[](size_t index) const {

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

    /*!
     * Returns whether the array is empty.
     *
     * @return Whether the array is empty.
     */
    FORCEINLINE bool isEmpty() const {
        return count == 0;
    }

    /*!
     * Returns the number of elements contained in the array.
     *
     * @return The number of elements in the array.
     */
    FORCEINLINE size_t getSize() const {
        return count;
    }

    /*!
     * Returns the number of elements the array can hold without resizing the data buffer.
     *
     * @return The array capacity.
     */
    FORCEINLINE size_t getCapacity() const {
        return capacity;
    }

private:

    /*!
     *  The data contained in the array.
     */
    T *data = nullptr;
    
    /*!
     * The number of elements in the array.
     */
    size_t count = 0;
    
    /*!
     * The size of the data buffer.
     */
    size_t capacity = 0;
    
    /*!
     * The allocator used to allocate and resize buffers.
     */
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

    explicit AutoReleaseArray(size_t capacity = 10) : Array<T>(capacity) {

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
