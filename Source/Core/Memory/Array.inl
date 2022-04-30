#pragma once

#ifndef __ARRAY_INCL
#error This file sould only be included from Array.h
#endif

template<typename T>
Array<T>::Array(const size_t capacity) : size(0), capacity(capacity) {

    data = allocator.alloc(capacity);

}

template<typename T>
Array<T>::Array(Array<T> const &other) {

    *this = other;

}

template<typename T>
Array<T>::Array(Array<T> &&other) {

    *this = std::move(other);

}

template<typename T>
Array<T>::Array(Array<T> const &other, size_t limit){
    
    size_t arrSize = other.size;
    
    // If the limit is above the array size,
    if(limit > arrSize){
        
        // Set the new size to be the array size.
        limit = arrSize;
        
    }
    
    // Set the size and capacity
    this->size = limit;
    this->capacity = limit;

    // Allocate our data.
    this->data = allocator.alloc(capacity);

    // Copy the data.
    allocator.copy(other.data, other.data + limit, this->data);
    
}

template<typename T>
Array<T>::~Array() {

    allocator.release(data);
    size = 0;
    capacity = 0;

}

template<typename T>
void Array<T>::append(T const &elem) {

    // Extend the array if necessary.
    extend();

    // Store the new element.
    data[size] = std::move(elem);

    // Increase the number of held objects.
    ++size;

}


template<typename T>
void Array<T>::append(Array<T> const &arr) {

    if (arr.isEmpty()) {
        return;
    }

    // Extend the array if necessary.
    extend(arr.getSize());

    // Store the new elements.
    allocator.copy(arr.data, arr.data + arr.getSize(), data + getSize());

    // Increase the number of held objects.
    size += arr.getSize();

}

template<typename T>
Optional<T> Array<T>::removeAt(size_t index) {

    Optional<T> opt = Optional<T>();

    // If the index is out of bounds, return.
    if (index >= size) {
        return opt;
    }

    // Move the data in the optional.
    opt = std::move(data[index]);

    // Move back all the elements after the index.
    allocator.move(data + index + 1, data + size, data + index);

    // Reduce the size of the array.
    --size;

    // If T is a primitive type, set the memory after the moved elements to zero.
    if (std::is_fundamental_v<T>) {
        std::memset(data + size, 0, capacity - size);
    }

    return opt;

}

template<typename T>
void Array<T>::reverse() {

    for (int i = 0; i < getSize() / 2; ++i) {
        allocator.swapMove(data + i, data + getSize() - 1 - i);
    }

}

template<typename T>
bool Array<T>::removeFirstOf(T const &elem) {

    Optional<size_t> index = firstIndexOf(elem);

    if (index.hasValue()) {

        removeAt(index.getValue());
        return true;

    }

    return false;

}

template<typename T>
bool Array<T>::removeLastOf(T const &elem) {

    Optional<size_t> index = lastIndexOf(elem);

    if (index.hasValue()) {

        removeAt(index.getValue());
        return true;

    }

    return false;

}

template<typename T>
void Array<T>::removeAllOf(T const &elem) {

    Array<size_t> indices = Array<size_t>(size / 4);

    for (size_t i = 0; i < size; ++i) {
        if (elem == data[i]) {
            indices += i;
        }
    }

    indices.sortDescending();

    for (auto &e: indices) {
        removeAt(e);
    }

}

template<typename T>
Optional<size_t> Array<T>::firstIndexOf(T const &elem) {

    size_t counter = 0;

    for (auto &e: *this) {
        if (e == elem) {
            return Optional<size_t>(counter);
        }
        ++counter;
    }

    return Optional<size_t>::empty();

}

template<typename T>
Optional<size_t> Array<T>::lastIndexOf(T const &elem) const {

    for (Int64 i = size - 1; i >= 0; --i) {
        if (data[i] == elem) {
            return { i };
        }
    }

    return Optional<size_t>::empty();

}

template<typename T>
Array<T> Array<T>::arrayWithRange(size_t from, size_t to){
    
    // Create a new array
    Array<T> array = Array<T>();

    // If the input parameter is invalid, return an empty array.
    if(from >= size || from >= to) { return array; }
    
    // If to is greater than the array size, set it to the array size.
    if(to > size) { to = size; }
    
    // Allocate data for the new array.
    array.data = allocator.alloc(to - from);
    
    // Copy the data to the new array.
    allocator.copy(data + from, data + to, array.data);
    
    // Set the size and capacity.
    array.size = to - from;
    array.capacity = to - from;
    
    // Return the array.
    return array;
    
}

template<typename T>
Array<T> Array<T>::arrayMatching(std::function<bool(T const &)> condition) {

    // Create a new array
    Array<T> array = Array<T>();

    // For each element
    for (auto &e: *this) {

        // If the condition is met
        if (condition(e)) {

            // Append the element to the new array
            array += e;

        }

    }

    // Return the array.
    return array;

}

template<typename T>
void Array<T>::removeDuplicates() {

    // Get an allocator to create a T** buffer.
    Allocator<T *> al;

    // Use raw buffer to avoid recursive template instantiation if using Array<T*>
    // We store pointers to the elements in our buffer to avoid copying objects.
    T **addresses = al.alloc(size);

    // The array that will store the duplicate indices.
    Array<size_t> duplicates = Array<size_t>(size / 4);

    // For each element in the array
    for (size_t i = 0; i < size; ++i) {

        // For each of the seen elements
        for (size_t j = 0; j < i; ++j) {

            // If a previously seen element is found
            if (*(addresses[j]) == data[i]) {

                // Add the index to the duplicates.
                duplicates += i;
                break;

            }

        }

        // Store the address of the value in the buffer.
        addresses[i] = data + i;

    }

    // Release the buffer.
    al.release(addresses);

    // Sort the array in descending order.
    duplicates.sortDescending();

    // Remove the duplicates.
    for (auto &e: duplicates) {
        removeAt(e);
    }

}

template<typename T>
void Array<T>::sortAscending() {
    internalSortAsc(0, static_cast<Int64>(getSize() - 1));
}

template<typename T>
void Array<T>::sortDescending() {
    internalSortDesc(0, static_cast<Int64>(getSize() - 1));
}

template<typename T>
bool Array<T>::contains(T const &elem) {

    for (auto &e: *this) {
        if (e == elem) {
            return true;
        }
    }

    return false;

}

template<typename T>
void Array<T>::insert(T const &elem, size_t index) {

    // If the index to insert is greater to the array size,
    if (index > size) {

        // Simply append the element to the array.
        append(elem);
        return;

    }

    // Extend the array if necessary.
    extend();

    // Move all the elements forward.
    allocator.move(data + index, data + size, data + index + 1);

    // Store the new element.
    data[index] = std::move(elem);

    // Increase the size of the array
    ++size;

}


template<typename T>
void Array<T>::insert(Array<T> const &arr, size_t index) {

    // If the index to insert is greater to the array size,
    if (index > size) {

        // Simply append the array to the array.
        append(arr);
        return;

    }

    // Extend the array if necessary.
    extend(arr.getSize());

    // Move all the elements forward.
    allocator.move(data + index, data + size, data + index + arr.getSize());

    // Store the new elements.
    allocator.copy(arr.data, arr.data + arr.getSize(), data + index);

    // Increase the size of the array
    size += arr.getSize();

}

template<typename T>
void Array<T>::resize(size_t newCap) {

    // Resize the data buffer.
    allocator.realloc(data, capacity, newCap);

    // Set the new capacity
    this->capacity = newCap;

    // Set the size if necessary
    if (size > newCap) {size = newCap;}

}

template<typename T>
void Array<T>::shrink() {

    // If the array can't be shrunk, return.
    if (size == capacity) {return;}

    if (size == 0) {
        clear();
        return;
    }

    // Shrink the array to the number of elements contained.
    allocator.realloc(data, capacity, size);
    this->capacity = size;

}

template<typename T>
void Array<T>::clear() {

    // Clear the array.
    allocator.release(this->data);

    // Reset the variables.
    this->size = 0;
    this->capacity = 0;

}

template<typename T>
Array<T> &Array<T>::operator=(Array<T> const &other) {

    // Deallocate the data in this array
    allocator.release(data);

    // If the other array is empty, set this array to empty.
    if (!other.data) {

        this->data = nullptr;
        this->size = 0;
        this->capacity = 0;

    } else {

        // Otherwise, allocate a new data buffer.
        this->data = allocator.alloc(other.capacity);

        // Copy the array contents.
        allocator.copy(other.data, other.data + other.size, this->data);

        // Store the new information.
        this->capacity = other.capacity;
        this->size = other.size;

    }

    return *this;

}

template<typename T>
Array<T> &Array<T>::operator=(Array<T> &&other) {

    allocator.release(data);

    this->data = other.data;
    this->capacity = other.capacity;
    this->size = other.size;

    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;

    return *this;

}

template<typename T>
T& Array<T>::getAt(size_t index) const {
    if(index >= capacity){
        abort();
        throw std::runtime_error("Array out of bounds.");
    }
    return data[index];
}

template<typename T>
T& Array<T>::operator[](size_t index) const {
    return getAt(index);
}

template<typename T>
void Array<T>::operator+=(T const &elem) {
    append(elem);
}

template<typename T>
void Array<T>::operator+=(Array<T> const &arr) {
    append(arr);
}

template<typename T>
bool Array<T>::operator==(Array<T> const& other) const{
    
    // If the sizes are different, return false.
    if (getSize() != other.getSize()) {
        return false;
    }

    // Compare each element.
    for (size_t i = 0; i < getSize(); ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }

    return true;
    
}

template<typename T>
bool Array<T>::operator!=(Array<T> const& other) const{
    
    // If the sizes are different, return true.
    if (getSize() != other.getSize()) {
        return true;
    }

    // Compare each element.
    for (size_t i = 0; i < getSize(); ++i) {
        if (data[i] != other.data[i]) {
            return true;
        }
    }

    return false;
    
}

template<typename T>
T *Array<T>::begin() const {
    return this->data;
}

template<typename T>
T *Array<T>::end() const {
    return this->data + size;
}

template<typename T>
void Array<T>::extend(const size_t ext) {

    // If the array is full
    if (ext && ((size + ext) > capacity)) {

        size_t newSize = (capacity + ext) * 2;

        // Resize our data buffer.
        allocator.realloc(data, capacity, newSize);

        this->capacity = newSize;

    }

}

template<typename T>
void Array<T>::internalSortAsc(Int64 from, Int64 to) {

    if (from < to) {

        Int64 idx = partitionAsc(from, to);

        internalSortAsc(from, idx - 1);
        internalSortAsc(idx + 1, to);

    }

}

template<typename T>
void Array<T>::internalSortDesc(Int64 from, Int64 to) {

    if (from < to) {

        Int64 idx = partitionDesc(from, to);

        internalSortDesc(from, idx - 1);
        internalSortDesc(idx + 1, to);

    }

}

template<typename T>
Int64 Array<T>::partitionAsc(const Int64 from, const Int64 to) {

    Int64 idx = from - 1;

    for (Int64 i = from; i <= to - 1; ++i) {

        if (data[i] < data[to]) {

            ++idx;
            allocator.swapMove(data + idx, data + i);

        }

    }

    allocator.swapMove(data + idx + 1, data + to);

    return idx + 1;

}

template<typename T>
Int64 Array<T>::partitionDesc(const Int64 from, const Int64 to) {

    Int64 idx = from - 1;

    for (Int64 i = from; i <= to - 1; ++i) {

        if (data[to] < data[i]) {

            ++idx;
            allocator.swapMove(data + idx, data + i);

        }

    }

    allocator.swapMove(data + idx + 1, data + to);

    return idx + 1;

}