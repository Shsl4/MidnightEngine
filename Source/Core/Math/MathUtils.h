#pragma once

#include <Core/EngineTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>

class Math {

public:
	
	template<typename NumberType>
    FORCEINLINE static NumberType clamp(NumberType a, NumberType min, NumberType max) {
		return a > max ? max : (a < min ? min : a);
	}

};
