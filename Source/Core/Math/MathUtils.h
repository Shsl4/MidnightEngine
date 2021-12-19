#pragma once

#include <Core/EngineTypes.h>

class Math {

public:
	
	template<typename NumberType>
    FORCEINLINE static NumberType clamp(NumberType a, NumberType min, NumberType max) {
		return a > max ? max : (a < min ? min : a);
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
