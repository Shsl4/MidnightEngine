#pragma once

#include <Core/EngineTypes.h>
#include "../Math/MathUtils.h"
struct LinearColor {

public:

	FORCEINLINE static LinearColor fromLinearRGBA(float r, float g, float b, float a) {

		LinearColor color;
		color.r = Math::clamp(r, 0.0f, 1.0f);
		color.g = Math::clamp(g, 0.0f, 1.0f);
		color.b = Math::clamp(b, 0.0f, 1.0f);
		color.a = Math::clamp(a, 0.0f, 1.0f);

		return color;

	}

	FORCEINLINE static LinearColor fromLinearRGB(float r, float g, float b) {

		return fromLinearRGBA(r, g, b, 1.0f);

	}

	FORCEINLINE static LinearColor fromRGBA(float r, float g, float b, float a) {

		LinearColor color;
		color.r = Math::clamp(r / 255.0f, 0.0f, 1.0f);
		color.g = Math::clamp(g / 255.0f, 0.0f, 1.0f);
		color.b = Math::clamp(b / 255.0f, 0.0f, 1.0f);
		color.a = Math::clamp(a, 0.0f, 1.0f);

		return color;

	}

	FORCEINLINE static LinearColor fromRGB(float r, float g, float b) {

		return fromRGBA(r, g, b, 1.0f);

	}

	FORCEINLINE float getRed() { return this->r; }

	FORCEINLINE float getGreen() { return this->g; }

	FORCEINLINE float getBlue() { return this->b; }

	FORCEINLINE float getAlpha() { return this->a; }

	FORCEINLINE void setRed(float r) { this->r = Math::clamp(r, 0.0f, 1.0f); }

	FORCEINLINE void setGreen(float g) { this->g = Math::clamp(g, 0.0f, 1.0f); }

	FORCEINLINE void setBlue(float b) { this->b = Math::clamp(b, 0.0f, 1.0f); }

	FORCEINLINE void setAlpha(float a) { this->a = Math::clamp(a, 0.0f, 1.0f); }

private:

	LinearColor();

	float r;
	float g;
	float b;
	float a;

};

struct LinearColors {

public:

	const static inline LinearColor transparent = LinearColor::fromLinearRGBA(0.0f, 0.0f, 0.0f, 0.0f);
	const static inline LinearColor black = LinearColor::fromLinearRGBA(0.0f, 0.0f, 0.0f, 1.0f);
	const static inline LinearColor white = LinearColor::fromLinearRGBA(1.0f, 1.0f, 1.0f, 1.0f);

	const static inline LinearColor red = LinearColor::fromLinearRGBA(1.0f, 0.0f, 0.0f, 1.0f);
	const static inline LinearColor green = LinearColor::fromLinearRGBA(0.0f, 1.0f, 0.0f, 1.0f);
	const static inline LinearColor blue = LinearColor::fromLinearRGBA(0.0f, 0.0f, 1.0f, 1.0f);


private:

	LinearColors() {

	}

};
