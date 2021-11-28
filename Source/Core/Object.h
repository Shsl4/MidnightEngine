#pragma once

#include <typeinfo>

class Object{
    
public:
    
    virtual ~Object() = default;
    
    virtual const char* getDescription(){
        
        return typeid(*this).name();
        
    }
        
};
