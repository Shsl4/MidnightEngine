#ifdef _WIN64

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#include <Core/MidnightEngine.h>
#include <Math/MathUtils.h>

int main(int argc, char** argv) {
	
#ifdef _WIN64

	// Enables memory leak check on Windows
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif // _WIN64

	UniquePtr<MidnightEngine> engine = std::make_unique<MidnightEngine>();

	int ret = -1;

	if(engine)
	{
		ret = engine->init(argc, argv);
		glfwTerminate();
	}
	
	return ret;

}
