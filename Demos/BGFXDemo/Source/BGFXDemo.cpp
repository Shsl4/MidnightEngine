#define SDL_MAIN_HANDLED
#include <Core/BGFXEngine.h>

int main(int argc, char** argv) {

	BGFXEngine engine = BGFXEngine();

	engine.init(argc, argv);

	return 0;
}
