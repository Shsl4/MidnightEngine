#pragma once

struct Vector3 {

public:

	Vector3() : x(0.0f), y(0.0f), z(0.0f){
	}

	Vector3(float v) : x(v), y(v), z(v) {
	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {
	}

	float x;
	float y;
	float z;
    
};
