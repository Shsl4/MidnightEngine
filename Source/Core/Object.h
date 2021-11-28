#pragma once

#include <typeinfo>

class Object{
    
public:
    
    virtual ~Object() = default;
    
    virtual const char* getDescription(){
        
        return typeid(*this).name();
        
    }

    template<class T>
    T* cast() {

        return dynamic_cast<T*>(this);

    }

    template<class T>
    bool instanceOf() {

        return cast<T>() != nullptr;

    }
        
};
