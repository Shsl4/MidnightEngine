#pragma once

#include <Core/EngineMacros.h>
#include <Logging/Logger.h>

class CommandError : public std::runtime_error {

public:
    
    using Super = std::runtime_error;

    explicit CommandError(const std::string& message) : Super(message.c_str()) {}

    explicit CommandError(const char* message) : Super(message) {}

    template<typename ... Args>
    FORCEINLINE static void throwIf(const bool condition, const char* format, Args &&... args)
    {
        if (condition)
        {
            throw CommandError(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
        }
    }
    
};
