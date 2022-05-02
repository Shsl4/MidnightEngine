#pragma once

#include <Core/EngineMacros.h>
#include <memory>

class Memory {
    
public:
    
    /*!
     *  Moves memory forwards or backwards automatically.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    template<typename T>
    void static move(T *from, T *to, T *dest);

    /*!
     *  Copies memory forwards or backwards automatically.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    template<typename T>
    void static copy(const T *from, const T *to, T *dest);

    /*!
     * Swaps the contents of the two pointers by moving (no copying).
     */
    template<typename T>
    FORCEINLINE void static swapMove(T *a, T *b);
    
    /*!
     *  Moves memory forward.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    template<typename T>
    FORCEINLINE static void moveForward(T *from, T *to, T *dest);

    /*!
     *  Moves memory backwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    template<typename T>
    FORCEINLINE static void moveBack(T *from, T *to, T *dest);

    /*!
     *  Copies memory forwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    template<typename T>
    FORCEINLINE static void copyForward(const T *from, const T *to, T *dest);

    /*!
     *  Copies memory backwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    template<typename T>
    FORCEINLINE static void copyBack(const T *from, const T *to, T *dest);
    
};

#define __MEMORY_INCL

#include <Memory/Memory.inl>

#undef __MEMORY_INCL
