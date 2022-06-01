#pragma once

#include <Core/Object.h>
#include <Memory/String.h>

/**
 *  \brief A struct that holds information of a single stack frame including the function name, file name,
 *  line number and library name.
 */
struct ENGINE_API StackFrame {
    
    StackFrame() = default;

    StackFrame(const char* function, const char* file, const char* library, Int64 line);

    bool operator==(const StackFrame& other) const;

    bool operator!=(const StackFrame& other) const;

    bool operator<(const StackFrame& other) const;

    /*!
     *  \brief Formats the stack frame into a readable string.
     */
    NODISCARD String format() const;
    
    String functionName;
    String fileName;
    String filePath;
    String libraryName;
    Int64 lineNumber = -1;

};

/**
 *  \brief A utility class that allows to get the current stack frames.
 */
class ENGINE_API StackTrace {

public:

    StackTrace() = delete;

    /*!
     *  \brief Creates a StackFrame array from the current stack.
     *
     *  \return The StackFrame array
     */
    static Array<StackFrame> getStackTrace();
    
};