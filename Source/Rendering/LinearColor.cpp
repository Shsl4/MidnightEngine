#include <Rendering/LinearColor.h>

LinearColor::LinearColor() {

	this->r = this->g = this->b = 0.0f;
	this->a = 1.0f;

}

LinearColor LinearColor::fromLinearRGBA(float r, float g, float b, float a) {

	LinearColor color;
	color.r = Math::clamp(r, 0.0f, 1.0f);
	color.g = Math::clamp(g, 0.0f, 1.0f);
	color.b = Math::clamp(b, 0.0f, 1.0f);
	color.a = Math::clamp(a, 0.0f, 1.0f);

	return color;

}

LinearColor LinearColor::fromLinearRGB(float r, float g, float b) {

	return fromLinearRGBA(r, g, b, 1.0f);

}

LinearColor LinearColor::fromRGBA(float r, float g, float b, float a) {

	LinearColor color;
	color.r = Math::clamp(r / 255.0f, 0.0f, 1.0f);
	color.g = Math::clamp(g / 255.0f, 0.0f, 1.0f);
	color.b = Math::clamp(b / 255.0f, 0.0f, 1.0f);
	color.a = Math::clamp(a, 0.0f, 1.0f);

	return color;

}

LinearColor LinearColor::fromRGB(float r, float g, float b) {

	return fromRGBA(r, g, b, 1.0f);

}
