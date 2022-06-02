#pragma once

#include <Memory/Array.h>

class ArrayUtils {

public:

    template<typename T>
    static void replaceAll(Array<T>& arr, T const& replace, T const &with) {

        for (auto& e: arr) {
            
            if (e == replace) {
                
                e = with;
                
            }
        }
        
    }
    
    template<typename T>
    static bool contains(Array<T> const& arr, T const &elem) {

        for (auto const& e: arr) {
            if (e == elem) {
                return true;
            }
        }

        return false;

    }

    template<typename T>
    static Optional<size_t> firstIndexOf(Array<T> const& arr, T const &elem) {

        size_t counter = 0;

        for (const auto &e: arr) {
            if (e == elem) {
                return { counter };
            }
            ++counter;
        }

        return Optional<size_t>::empty();

    }

    template<typename T>
    static Optional<size_t> lastIndexOf(Array<T> const& arr, T const &elem) {

        for (Int64 i = arr.getSize() - 1; i >= 0; --i) {
            if (arr[i] == elem) {
                return { i };
            }
        }

        return Optional<size_t>::empty();

    }

    template<typename T>
    static bool removeFirstOf(Array<T>& arr, T const &elem) {

        Optional<size_t> index = ArrayUtils::firstIndexOf(arr, elem);

        if (index.hasValue()) {

            arr.removeAt(index.getValue());
            return true;

        }

        return false;

    }

    template<typename T>
    static bool removeLastOf(Array<T> const& arr, T const &elem) {
        
        Optional<size_t> index = ArrayUtils::lastIndexOf(arr, elem);

        if (index.hasValue()) {

            arr.removeAt(index.getValue());
            return true;

        }

        return false;

    }
    
};