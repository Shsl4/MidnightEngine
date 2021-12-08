#pragma once

#include <typeinfo>

class Object{
    
public:
    
    virtual ~Object() = default;
    
    const char* getClassName() {

        return typeid(*this).name();

    }

    virtual const char* getDescription(){
        
        return getClassName();
        
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
