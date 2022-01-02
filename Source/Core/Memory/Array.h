#pragma once

#include <Core/EngineTypes.h>
#include <Memory/Allocator.h>

template<typename T>
struct Array{
    
    explicit Array(size_t capacity = 15){
        
        this->capacity = capacity;
        this->data = allocator.allocate<T>(capacity);
        memset(data, 0, capacity * sizeof(T));
        
    }
    
    virtual ~Array(){
        
        count = 0;
        capacity = 0;
        allocator.deallocate(data);
        
    }
    
    template<typename S>
    void append(S const& other) {
        
        if(count == capacity){
            this->data = allocator.resize(data, count == 0 ? 15 : capacity * 2);
            capacity *= 2;
        }
        
        this->data[count] = other;
        ++count;
        
    }
    
    template<typename S>
    bool remove(S const& other){
                        
        for(size_t i = 0; i < count; ++i){
            
            if(other == data[i]){

                for(size_t j = i; j < count - 1; ++j){
                    
                    data[j] = data[j + 1];
                    
                }
                
                --count;
                memset(&data[count], 0, sizeof(T));
                return true;
                
            }
            
        }
        
        return false;
        
    }
    
    template<typename S>
    bool contains(S const& other) const {
        
        for(size_t i = 0; i < count; ++i){
            
            if(other == data[i]){
                
                return true;
                
            }
            
        }
        
        return false;
        
    }
    
    void clear() {
        
        memset(data, 0, capacity * sizeof(T));
        this->count = 0;
        
    }
    
    void shrink() {
        
        if(count == capacity) { return; }
        this->data = allocator.resize(data, count);
        this->capacity = count;
        
    }
    
    void resize(size_t size){
        
        this->data = allocator.resize(data, size);
        this->capacity = size;
        if(count > size) { count = size; }
        
    }
    
    T* begin() const {

        return this->data;

    }

    T* end() const {

        return this->data + count;

    }
    
    void print(){
                  
        for (int i = 0; i < count; ++i) {
              
            std::cout << data[i] << " ";
              
        }
        
        std::cout << std::endl;
          
    }
    
    T* getRawData() const{
        
        return this->data;
        
    }
    
    T operator[](size_t index) const {
        
        return data[index];
        
    }
    
    template<typename S>
    void operator+=(S const& other) {
        
        append(other);
        
    }
    
    FORCEINLINE bool isEmpty() const { return count == 0; }
    FORCEINLINE size_t getSize() const { return count; }
    FORCEINLINE size_t getCapacity() const { return capacity; }

protected:
    
    T* data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
    Allocator allocator = Allocator();
      
};
