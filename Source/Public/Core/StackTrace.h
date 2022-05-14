#pragma once

#include <Core/Object.h>
#include <Memory/String.h>

struct ENGINE_API StackFrame {
    
    StackFrame() = default;

    StackFrame(const char* function, const char* file, const char* library, size_t line);

    bool operator==(const StackFrame& other) const;

    bool operator!=(const StackFrame& other) const;

    bool operator<(const StackFrame& other) const;
    
    String functionName;
    String fileName;
    String libraryName;
    UInt64 lineNumber = -1;

};

class ENGINE_API StackTrace : public Object {

public:

    StackTrace() = delete;

    static Array<StackFrame> getStackTrace();
    
};