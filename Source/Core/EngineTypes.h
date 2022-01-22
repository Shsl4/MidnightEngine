#pragma once

#include <iostream>
#include <memory>
#include <stdint.h>

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

using Int = int32_t;
using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using UInt = uint32_t;
using UInt8 = uint8_t;
using UInt16 = uint16_t;
using UInt32 = uint32_t;
using UInt64 = uint64_t;

#ifndef _WIN64
#define FORCEINLINE __attribute__((always_inline))
#else
#define FORCEINLINE __forceinline
#endif // !WIN64

