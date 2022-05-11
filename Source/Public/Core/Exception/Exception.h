#pragma once

#include <stdexcept>
#include <Core/EngineMacros.h>
#include <Logging/Logger.h>

#include <source_location>

class Exception : public std::runtime_error {

public:
    
    using Super = std::runtime_error;

    Exception(const std::string& message, std::source_location loc = std::source_location::current())
        : Super(message.c_str()), location(loc) {}

    Exception(const char* message, std::source_location loc = std::source_location::current())
        : Super(message), location(loc) {}

    template <typename ... Args>
    FORCEINLINE static void throwIf(const bool condition, const char* format, Args&&... args) {
        if (condition) { throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...))); }
    }

    template <typename ... Args>
    NORETURN FORCEINLINE static void throwError(const char* format, Args&&... args) {
        throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
    }

    template <typename ... Args>
    FORCEINLINE static void throwIfLocated(const bool condition, const char* format, std::source_location loc, Args&&... args) {
        if (condition) { throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)), loc); }
    }

    template <typename ... Args>
    NORETURN FORCEINLINE static void throwErrorLocated(const char* format, std::source_location loc, Args&&... args) {
        throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)), loc);
    }

    std::source_location location;
    
};

#define DEFINE_EXCEPTION(name)                                                                                  \
                                                                                                                \
    class name : public Exception {                                                                             \
                                                                                                                \
    public:                                                                                                     \
                                                                                                                \
        using Super = Exception;                                                                                \
                                                                                                                \
        explicit name(const std::string& message, std::source_location loc = std::source_location::current())   \
                : Super(message.c_str()), location(loc) {}                                                      \
                                                                                                                \
        explicit name(const char* message, std::source_location loc = std::source_location::current())          \
                : Super(message), location(loc) {}                                                              \
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
        std::source_location location;                                                                          \
    }                                                                                                           \
                                                                                                                \

DEFINE_EXCEPTION(NullPointerException);
DEFINE_EXCEPTION(CommandError);
DEFINE_EXCEPTION(ParseError);

#define expect(condition, format, ...) Exception::throwIfLocated(!condition, format, std::source_location::current(), __VA_ARGS__)
#define raise(format, ...) Exception::throwErrorLocated(format, std::source_location::current(), __VA_ARGS__)
