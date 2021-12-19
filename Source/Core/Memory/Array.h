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
    
    explicit Array(size_t capacity = 15){
    
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
        allocator.deallocate(data);
                
    }
    
    template<class S>
    bool append(S* other){
        
        static_assert(std::is_base_of_v<T, S>, "S should inherit from T");
        
        if(count == capacity) {
        
            this->data = allocator.resize(this->data, capacity * 2);
            
            for(size_t i = capacity; i < capacity * 2; ++i){
                data[i] = nullptr;
            }
            
            capacity *= 2;
            
        }
                
        data[count] = other;
        ++count;
        
        return true;
        
    }

    template<class S>
    bool remove(S* other){
                
        int index;
        
        for (index = 0; index < count; ++index) {
            
            if(other == data[index]){
                
                break;
                
            }
            
        }

        if(index == count) { return false; }
                
        data[index] = nullptr;
        --count;

        size_t i;

        for (i = index; i < count; ++i) {
            
            data[i] = data[i + 1];
            
        }
        
        data[i] = nullptr;
        
        return true;
        
    }
    
    bool remove(size_t index, T** out = nullptr){
                
        if(index >= count) { return false; }
        
        if(out) {
            
            *out = data[index];
            
        }
                
        data[index] = nullptr;
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
        
        return data[index];
        
    }
    
    bool operator+=(T& other){
        
        return append(other);
        
    }

    T** begin() {

        return this->data;

    }

    T** end() {

        return this->data + count;

    }
    
    FORCEINLINE size_t getCount() const { return count; }
    FORCEINLINE size_t getCapacity() const { return capacity; }
    
private:
    
    T** data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Allocator allocator = Allocator();
   
};

template<typename T>
class ManagedArray {

public:
    
    explicit ManagedArray(size_t capacity = 15) {

        data = allocator.allocate<T*>(capacity);

        for (int i = 0; i < capacity; ++i) {

            data[i] = nullptr;

        }

        this->count = 0;
        this->capacity = capacity;

    }

    ~ManagedArray() {

        clear();
        capacity = 0;
        allocator.deallocate(data);

    }

    template<class S>
    bool append(S* other) {

        static_assert(std::is_base_of_v<T, S>, "S should inherit from T");

        if (count == capacity) {

            this->data = allocator.resize(this->data, capacity * 2);

            for (size_t i = capacity; i < capacity * 2; ++i) {
                data[i] = nullptr;
            }

            capacity *= 2;

        }

        data[count] = other;
        ++count;

        return true;

    }

    template<class S>
    bool remove(S* other) {

        int index;

        for (index = 0; index < count; ++index) {

            if (other == data[index]) {

                break;

            }

        }

        if (index == count) { return false; }

        allocator.deallocate(data[index]);
        data[index] = nullptr;
        --count;

        size_t i;

        for (i = index; i < count; ++i) {

            data[i] = data[i + 1];

        }

        data[i] = nullptr;

        return true;

    }

    bool remove(size_t index, T** out = nullptr) {

        if (index >= count) { return false; }

        if (out) {

            *out = data[index];

        }
        else {

            allocator.deallocate(data[index]);

        }

        data[index] = nullptr;
        --count;

        size_t i;

        for (i = index; i < count; ++i) {

            data[i] = data[i + 1];

        }

        data[i] = nullptr;

        return true;

    }

    void clear() {

        for (int i = 0; i < capacity; ++i) {

            allocator.deallocate(data[i]);
            data[i] = nullptr;

        }

        count = 0;

    }

    void shrink() {

        if (count == capacity) { return; }
        this->data = allocator.resize(this->data, count);
        this->capacity = count;

    }

    void print() {

        for (int i = 0; i < capacity; ++i) {

            std::cout << i << ": ";

            if (data[i] == nullptr) {
                std::cout << "nullptr" << std::endl;
            }
            else {
                std::cout << data[i] << std::endl;
            }

        }

        std::cout << std::endl;

    }

    T* operator[](size_t index) {

        return data[index];

    }

    bool operator+=(T& other) {

        return append(other);

    }

    T** begin() const {

        return data;

    }

    T** end() const {

        return data + count;

    }

    FORCEINLINE size_t getCount() const { return count; }
    FORCEINLINE size_t getCapacity() const { return capacity; }

private:

    T** data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Allocator allocator = Allocator();

};
