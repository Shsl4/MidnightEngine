#include <Core/ErrorHandler.h>
#include <Core/EngineTypes.h>

void ErrorHandler::onError(int code, const char* description)
{
	std::cout << "Error: " << description << " Code: " << code << std::endl;
}
