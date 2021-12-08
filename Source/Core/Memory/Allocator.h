//
//  Allocator.hpp
//  BGFXDemo
//
//  Created by Pierre on 14/11/2021.
//

#pragma once

#include <stdlib.h>

class Allocator{
    
public:
    
    Allocator(){
                
    }
    
    template<typename T>
    T* resize(T* ptr, size_t count){
        
        return (T*)realloc(ptr, sizeof(T) * count);
        
    }
    
    template<typename T>
    T* copy(T& other){
        
        return new T(other);
        
    }

    template <class T, class... Args>
    T* instantiate(Args&&... _Args) { 
        return new T(std::forward<Args>(_Args)...);
    }
    
    template<typename T>
    T* allocate(size_t count){
      
        return (T*)operator new(sizeof(T) * count);
        
    }
    
    template<typename T>
    void deallocate(T*& pointer){
        
        if (!pointer) return;
        delete pointer;
        pointer = nullptr;
        
    }
    
    
};