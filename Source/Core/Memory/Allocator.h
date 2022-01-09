#pragma once

#include <stdlib.h>

class Allocator{
    
public:
    
    Allocator() = default;
    
    template<typename T>
    T* resize(T* ptr, const size_t count){
        
        return static_cast<T*>(realloc(ptr, sizeof(T) * count));
        
    }
    
    template<typename T>
    T* copy(T& other){
        
        return new T(other);
        
    }

    template <class T, class... Args>
    T* instantiate(Args&&... args) { 
        return new T(std::forward<Args>(args)...);
    }
    
    template<typename T>
    T* allocate(size_t count) const {

        return static_cast<T*>(operator new(sizeof(T) * count));
        
    }
    
    template<typename T>
    void deallocate(T*& pointer){

        if (!pointer) return;
        delete pointer;
        pointer = nullptr;
        
    }
    
};
