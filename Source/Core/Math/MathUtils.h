#pragma once

#include <Core/EngineTypes.h>
#include <bit>
#include <limits>

/*!
 * A utility class containing templated math operations
 */
class Math {

public:

    template<typename NumberType>
    FORCEINLINE static NumberType clamp(NumberType a, NumberType min, NumberType max) {
        return a > max ? max : (a < min ? min : a);
    }

    template<typename NumberType>
    FORCEINLINE static NumberType clampAround(NumberType a, NumberType min, NumberType max) {

        if (a < min) {return max - fabs(fmod(a, max));}
        if (a > max) {return min + fabs(fmod(a, max));}

        return a;

    }

    FORCEINLINE static float fastInvSqrt(float number){
        
        const float x2 = number * 0.5f;
        const float threehalfs = 1.5f;
        
        union {
            
            float f;
            UInt32 i;
            
        } conv = { .f = number };
        
        conv.i  = 0x5f3759df - ( conv.i >> 1 );
        conv.f *= threehalfs - ( x2 * conv.f * conv.f );
        return conv.f;
        
    }

    template<typename NumberType>
    FORCEINLINE static NumberType max(NumberType a, NumberType b) {
        return a > b ? a : b;
    }

    template<typename NumberType>
    FORCEINLINE static NumberType max(NumberType a, NumberType b, NumberType c) {
        return a > b ? ((a > c) ? a : c) : ((b > c) ? b : c);
    }

    template<typename NumberType>
    FORCEINLINE static NumberType toRadians(NumberType degrees) {
        return degrees * static_cast<NumberType>(0.01745329252f);
    }

};
