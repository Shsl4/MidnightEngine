#pragma once

#ifndef __OPT_INCL
#error This file sould only be included from Optional.h
#endif

template<typename T>
Optional<T>::Optional(T &&v) {
    value = allocator.construct(v);
}

template<typename T>
Optional<T>::Optional(Optional<T> const &opt) {
    *this = opt;
}

template<typename T>
Optional<T>::Optional(T const &v) {
    value = allocator.construct(v);
}

template<typename T>
Optional<T>::Optional(T *v) {

    if (!v) {
        return;
    }
    value = allocator.construct(*v);

}

template<typename T>
template<typename ... Args>
Optional<T>::Optional(Args ...args) {

    value = allocator.construct(std::forward<Args>(args)...);

}

template<typename T>
Optional<T> Optional<T>::empty() {
    return Optional<T>();
}

template<typename T>
bool Optional<T>::hasValue() const {
    return value != nullptr;
}

template<typename T>
T& Optional<T>::getValue() const {

    if (!value) {
        throw std::runtime_error("Trying to access an empty optional value!");
    }

    return *value;

}

template<typename T>
T& Optional<T>::getValueElse(T &def) const {
    return value ? *value : def;
}

template<typename T>
T Optional<T>::getValueElse(T def) const {
    return value ? *value : def;
}

template<typename T>
T* Optional<T>::operator->() {
    
    if (!value) {
        throw std::runtime_error("Trying to access an empty optional value!");
    }
    
    return value;
}

template<typename T>
T& Optional<T>::operator()() const{
    
    if (!value) {
        throw std::runtime_error("Trying to access an empty optional value!");
    }
    
    return *value;
    
}

template<typename T>
bool Optional<T>::operator!() const{
    return !value;
}

template<typename T>
Optional<T> &Optional<T>::operator=(Optional<T> const &other) {

    allocator.destroy(value);

    if (other.hasValue()) {
        value = allocator.construct(other());
    }

    return *this;

}

template<typename T>
Optional<T> &Optional<T>::operator=(Optional<T> &&other) {

    allocator.destroy(value);
    value = other.value;
    other.value = nullptr;
    return *this;

}

template<typename T>
Optional<T> &Optional<T>::operator=(T &&other) {

    allocator.destroy(value);
    value = allocator.construct(other);
    return *this;

}

template<typename T>
Optional<T> &Optional<T>::operator=(T const &other) {

    allocator.destroy(value);
    value = allocator.construct(other);
    return *this;

}

template<typename T>
template<typename U>
bool Optional<T>::operator==(Optional<U> const &other) const {

    // If no value is present either of the optionals, return true.
    if (!value && !other.hasValue()) {
        return true;
    }

    // If only one of the values is present, return false.
    if ((value && !other.hasValue()) || (!value && other.hasValue())) {
        return false;
    }

    // Compare the values.
    return *value == other();

}

template<typename T>
template<typename U>
bool Optional<T>::operator<(Optional<U> const &other) const {

    // If no value is present either of the optionals, return false.
    if (!value && !other.hasValue()) {
        return false;
    }

    // If no value is present, return true.
    if (!value) {
        return true;
    }

    // If no value is present in other, return false.
    if (!other.hasValue()) {
        return false;
    }

    // Compare the values.
    return *value < other();

}

template<typename T>
template<typename U>
bool Optional<T>::operator>(Optional<U> const &other) const {

    // If no value is present either of the optionals, return false.
    if (!value && !other.hasValue()) {
        return false;
    }

    // If no value is present, return false.
    if (!value) {
        return false;
    }

    // If no value is present in other, return true.
    if (!other.hasValue()) {
        return true;
    }

    // Compare the values.
    return *value > other();

}

template<typename T>
template<typename U>
bool Optional<T>::operator<=(Optional<U> const &other) const {

    // If no value is present either of the optionals, return true.
    if (!value && !other.hasValue()) {
        return true;
    }

    // If no value is present, return true.
    if (!value) {
        return true;
    }

    // If no value is present in other, return false.
    if (!other.hasValue()) {
        return false;
    }

    // Compare the values.
    return *value <= other();

}

template<typename T>
template<typename U>
bool Optional<T>::operator>=(Optional<U> const &other) const {

    // If no value is present either of the optionals, return true.
    if (!value && !other.hasValue()) {
        return true;
    }

    // If no value is present, return false.
    if (!value) {
        return false;
    }

    // If no value is present in other, return true.
    if (!other.hasValue()) {
        return true;
    }

    // Compare the values.
    return *value >= other();

}

template<typename T>
template<typename U>
bool Optional<T>::operator==(U const &other) const {

    // If no value is present, return false.
    if (!value) {
        return false;
    }

    // Compare the values.
    return *value == other;

}

template<typename T>
template<typename U>
bool Optional<T>::operator<(U const &other) const {

    // If no value is present, return true.
    if (!value) {
        return true;
    }

    // Compare the values.
    return *value < other;

}

template<typename T>
template<typename U>
bool Optional<T>::operator>(U const &other) const {

    // If no value is present, return false.
    if (!value) {
        return false;
    }

    // Compare the values.
    return *value > other;

}

template<typename T>
template<typename U>
bool Optional<T>::operator<=(U const &other) const {

    // If no value is present, return true.
    if (!value) {
        return true;
    }

    // Compare the values.
    return *value <= other;

}

template<typename T>
template<typename U>
bool Optional<T>::operator>=(U const &other) const {

    // If no value is present, return false.
    if (!value) {
        return false;
    }

    // Compare the values.
    return *value >= other;

}

template<typename T>
Optional<T>::~Optional() {
    allocator.destroy(value);
}