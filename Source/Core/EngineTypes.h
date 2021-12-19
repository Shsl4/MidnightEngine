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

