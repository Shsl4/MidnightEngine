#pragma once

#include <stdexcept>
#include <Core/EngineMacros.h>
#include <Logging/Logger.h>

class Exception : public std::runtime_error {

public:
    
    Exception() = delete;

    NODISCARD class Array<struct StackFrame> getStackTrace() const { return this->stackTrace; }

    template <typename ... Args>
    FORCEINLINE static void throwIf(const bool condition, const char* format, Args&&... args);

    template <typename ... Args>
    NORETURN FORCEINLINE static void throwError(const char* format, Args&&... args);

protected:
    
    Exception(const std::string& message);

    Exception(const char* message);

private:
    
    Array<StackFrame> stackTrace;

};

#define __EXCEPTION_INCL
#include <Exception/Exception.inl>
#undef __EXCEPTION_INCL


#define DEFINE_EXCEPTION(name)                                                                                  \
                                                                                                                \
    class name : public Exception {                                                                             \
                                                                                                                \
    public:                                                                                                     \
                                                                                                                \
        using Super = Exception;                                                                                \
                                                                                                                \
        template<typename ... Args>                                                                             \
        FORCEINLINE static void throwIf(const bool condition, const char* format, Args &&... args) {            \
                                                                                                                \
           if (condition)                                                                                       \
           {                                                                                                    \
               throw name(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));            \
           }                                                                                                    \
                                                                                                                \
        }                                                                                                       \
                                                                                                                \
        template<typename ... Args>                                                                             \
        NORETURN FORCEINLINE static void throwError(const char* format, Args &&... args) {                      \
                                                                                                                \
            throw name(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));               \
                                                                                                                \
        }                                                                                                       \
                                                                                                                \
    protected:                                                                                                  \
                                                                                                                \
        name(const std::string& message) : Super(message.c_str()) {}                                            \
                                                                                                                \
        name(const char* message) : Super(message) {}                                                           \
                                                                                                                \
    }

DEFINE_EXCEPTION(OutOfBoundsException);

DEFINE_EXCEPTION(CommandError);
DEFINE_EXCEPTION(ParseError);

#define expectf(condition, format, ...) Exception::throwIf(!condition, format, __VA_ARGS__)
#define expect(condition, format) Exception::throwIf(!(condition), format)

#define raisef(format, ...) Exception::throwError(format, __VA_ARGS__)
//#define raise(format) Exception::throwError(format)
