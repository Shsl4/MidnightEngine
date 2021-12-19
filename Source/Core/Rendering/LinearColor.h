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

	FORCEINLINE float getRed() const { return this->r; }

	FORCEINLINE float getGreen() const { return this->g; }

	FORCEINLINE float getBlue() const { return this->b; }

	FORCEINLINE float getAlpha() const { return this->a; }

	FORCEINLINE void setRed(float red) { this->r = Math::clamp(red, 0.0f, 1.0f); }

	FORCEINLINE void setGreen(float green) { this->g = Math::clamp(green, 0.0f, 1.0f); }

	FORCEINLINE void setBlue(float blue) { this->b = Math::clamp(blue, 0.0f, 1.0f); }

	FORCEINLINE void setAlpha(float alpha) { this->a = Math::clamp(alpha, 0.0f, 1.0f); }

private:

	LinearColor() = default;

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;

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

	LinearColors() = default;

};
