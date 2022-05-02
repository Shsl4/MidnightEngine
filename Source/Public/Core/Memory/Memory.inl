#pragma once

#ifndef __MEMORY_INCL
#error This file sould only be included from Memory.h
#endif

template<typename T>
void Memory::move(T *from, T *to, T *dest) {

    if (dest > from) {
        moveForward(from, to, dest);
        return;
    }

    moveBack(from, to, dest);

}

template<typename T>
void Memory::copy(const T *from, const T *to, T *dest) {

    if (dest > from) {
        copyForward(from, to, dest);
        return;
    }

    copyBack(from, to, dest);

}

template<typename T>
void Memory::swapMove(T *a, T *b) {

    T temp = std::move(*a);
    *a = std::move(*b);
    *b = std::move(temp);

}

template<typename T>
void Memory::moveForward(T *from, T *to, T *dest) {

    dest += to - from - 1;

    while (from != to) {
        *dest = std::move(*(to - 1));
        --dest;
        --to;
    }

}

template<typename T>
void Memory::moveBack(T *from, T *to, T *dest) {

    while (from != to) {

        *dest = std::move(*from);
        ++dest;
        ++from;

    }

}

template<typename T>
void Memory::copyForward(const T *from, const T *to, T *dest) {

    dest += to - from - 1;

    while (from != to) {
        *dest = *(to - 1);
        --dest;
        --to;
    }

}

template<typename T>
void Memory::copyBack(const T *from, const T *to, T *dest) {

    while (from != to) {

        *dest = *from;
        ++dest;
        ++from;

    }

}
