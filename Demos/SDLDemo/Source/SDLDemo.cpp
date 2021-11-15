#ifdef _WIN64

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#include <Core/SDLEngine.h>

int main(int argc, const char** argv) {

#ifdef _WIN64

	// Enables memory leak check on Windows
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif // _WIN64

	SDL_SetMainReady();

	UniquePtr<SDLEngine> engine = std::make_unique<SDLEngine>();

	int ret = -1;

	if (engine)
	{
		ret = engine->init(argc, argv);
	}

	return ret;

}
