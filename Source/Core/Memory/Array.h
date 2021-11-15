//
//  Array.hpp
//  BGFXDemo
//
//  Created by Pierre on 14/11/2021.
//

#pragma once

#include <Core/EngineTypes.h>
#include <Memory/Allocator.h>

template<typename T>
class Array{
   
public:
    
    Array(size_t capacity = 15){
    
        data = allocator.allocate<T*>(capacity);
        
        for (int i = 0; i < capacity; ++i) {
            data[i] = nullptr;
        }
        
        this->count = 0;
        this->capacity = capacity;
                        
    }

    ~Array(){
        
        clear();
        capacity = 0;
        delete data;
                
    }
    
    template<class S>
    bool append(S& other){
        
        if(count == capacity) {
        
            this->data = allocator.resize(this->data, capacity * 2);
            
            for(size_t i = capacity; i < capacity * 2; ++i){
                data[i] = nullptr;
            }
            
            capacity *= 2;
            
        }
                
        data[count] = &other;
        ++count;
        
        return true;
        
    }
    
    bool remove(size_t index, T* out = nullptr){
                
        if(index > count) { return false; }
        
        if(out) { *out = *data[index]; }
                
        data[index] = nullptr;
        allocator.deallocate(data[index]);
        --count;

        size_t i;

        for (i = index; i < count; ++i) {
            
            data[i] = data[i + 1];
            
        }
        
        data[i] = nullptr;
        
        return true;
        
    }
    
    void clear(){
                
        for (int i = 0; i < capacity; ++i) {
            
            data[i] = nullptr;
            
        }
        
        count = 0;
        
    }
            
    void shrink(){
        
        if(count == capacity) { return; }
        this->data = allocator.resize(this->data, count);
        this->capacity = count;
        
    }
    
    void print(){
                
        for (int i = 0; i < capacity; ++i) {
            
            std::cout << i << ": ";
            
            if(data[i] == nullptr){
                std::cout << "nullptr" << std::endl;
            }
            else{
                std::cout << data[i] << std::endl;
            }
            
        }
        
        std::cout << std::endl;
        
    }
    
    T* operator[](size_t index) {
        
        assert(index < count, "Array out of bounds!");
        return data[index];
        
    }
    
    bool operator+=(T& other){
        
        return append(other);
        
    }
    
    FORCEINLINE const size_t getCount(){ return count; }
    FORCEINLINE const size_t getCapacity(){ return capacity; }
    
private:
    
    T** data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Allocator allocator = Allocator();
   
};
