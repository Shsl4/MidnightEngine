#pragma once

#ifndef __EXCEPTION_INCL
#error "This file should only be included from Exception.h"
#endif

#include <Core/StackTrace.h>

inline Exception::Exception(const std::string& message)
    : std::runtime_error(message.c_str()), stackTrace(StackTrace::getStackTrace()) {

    
}

inline Exception::Exception(const char* message): std::runtime_error(message), stackTrace(StackTrace::getStackTrace()) {}

template <typename ... Args>
void Exception::throwIf(const bool condition, const char* format, Args&&... args) {
    if (condition) { throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...))); }
}

template <typename ... Args>
void Exception::throwError(const char* format, Args&&... args) {
    throw Exception(fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...)));
}
