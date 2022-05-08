#pragma once

#include <Core/EngineMacros.h>
#include <Logging/Logger.h>

class ParseError : public std::runtime_error {

public:
    
    using Super = std::runtime_error;

    explicit ParseError(const std::string& message) : Super(message.c_str()) {}

    explicit ParseError(const char* message) : Super(message) {}

    template<typename ... Args>
    FORCEINLINE static void throwIf(const bool condition, const char* format, Args &&... args)
    {
        if (condition)
        {
            throw ParseError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
        }
    }

    template<typename ... Args>
    NORETURN FORCEINLINE static void throwError(const char* format, Args &&... args)
    {
        throw ParseError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
    }
    
};
