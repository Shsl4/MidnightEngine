#pragma once

#include <iostream>
#include <memory>
#include <thread>

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

#ifndef _WIN64
#define FORCEINLINE __attribute__((always_inline))
#else
#define FORCEINLINE __forceinline
#endif // !WIN64

#undef assert

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define assert(condition, message)                          \
                                                            \
if(!(condition)){                                           \
                                                            \
    std::cout << "Assertion failed in file: " << __FILE__   \
              << " function: " << __PRETTY_FUNCTION__       \
              << ", line: " << __LINE__                     \
              << ". Message: " << message                   \
              << std::endl;                                 \
                                                            \
    abort();                                                \
}
