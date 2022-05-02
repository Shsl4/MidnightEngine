#include <Core/Object.h>
#include <Memory/String.h>

#ifdef __GNUG__

#include <cxxabi.h>

String demangle(const char* name) {

    int status = -1;

    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);

    if(status == 0){
        String className = demangled;
        std::free(demangled);
        return className;
    }
    
    return name;
    
}

#endif

String Object::getClassName() const {

    const char* className = typeid(*this).name();
    
#ifdef __GNUG__
    return demangle(className);
#else
    return className;
#endif

}

String Object::getDescription() const {

    return getClassName();

}
