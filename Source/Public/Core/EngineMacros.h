#pragma once

#pragma warning(disable: 4251)

#ifndef _WIN64

#define FORCEINLINE __attribute__((always_inline)) inline
#define ENGINE_API 

#else

#ifdef ENGINE_EXPORT
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define FORCEINLINE __forceinline

#endif

#define ENGINE_INTERNAL

#define NODISCARD [[nodiscard]]
#define NORETURN [[noreturn]]
