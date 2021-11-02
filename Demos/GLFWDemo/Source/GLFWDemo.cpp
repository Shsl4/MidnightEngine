#ifdef _WIN64

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#include <Core/GLFWEngine.h>

int main(int argc, char** argv) {

#ifdef _WIN64

	// Enables memory leak check on Windows
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif // _WIN64

	UniquePtr<GLFWEngine> engine = std::make_unique<GLFWEngine>();

	int ret = -1;

	if (engine)
	{
		ret = engine->init(argc, argv);
		glfwTerminate();
	}

	return ret;

}
