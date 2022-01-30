#include <Core/Object.h>
#include <Memory/String.h>

#ifdef __GNUG__

#include <cxxabi.h>

String demangle(const char* name) {

    int status = -1;

    char* __dmgl = abi::__cxa_demangle(name, NULL, NULL, &status);

    if(status == 0){
        String className = __dmgl;
        std::free(__dmgl);
        return className;
    }
    
    return name;
    
}

#endif

String Object::getClassName() const {

    const char* __name = typeid(*this).name();
    
#ifdef __GNUG__
    return demangle(__name);
#endif
    
    return __name;

}

String Object::getDescription() const {

    return getClassName();

}
