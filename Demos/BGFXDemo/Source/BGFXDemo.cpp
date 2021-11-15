#define SDL_MAIN_HANDLED
#include <Core/BGFXEngine.h>

#ifndef __APPLE__

int main(int argc, const char** argv) {

	BGFXEngine engine = BGFXEngine();

	engine.init(argc, argv);

	return 0;
}

#endif