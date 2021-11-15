#ifdef _WIN64
#include <Windows.h>
#endif

#include <Core/GLFWEngine.h>
#include <Rendering/Vertex.h>
#include <Rendering/Triangle.h>
#include <Rendering/Grid.h>
#include <cmath>

int GLFWEngine::init(int argc, const char** argv)
{

#ifdef _WIN64

	// Enables ANSI escape codes on Windows
	SetConsoleMode(GetConsoleWindow(), ENABLE_VIRTUAL_TERMINAL_PROCESSING);

#endif

	getLogger()->info("Initializing MidnightEngine...");
    
	// Initialize GLFW
	if (!glfwInit()) {
		getLogger()->fatal("Failed to initialize GLFW!");
		return -1;
	}

	glfwSetErrorCallback(ErrorHandler::onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef _TRANSPARENT
    
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_DECORATED, false);
    
#endif

#ifdef FULLSCREEN

	GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);
	mainWindow = glfwCreateWindow(mode->width, mode->height, "MidnightEngine", mainMonitor, nullptr);

#else

    GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);
	mainWindow = glfwCreateWindow(500, 500, "MidnightEngine", nullptr, nullptr);
    glfwSetWindowPos(mainWindow, mode->width / 2 - 250, mode->height / 2 - 250);

#endif

	if (!mainWindow)
	{
		getLogger()->fatal("Failed to create window!");
		return -1;
	}

	glfwMakeContextCurrent(mainWindow);
	gladLoadGL();
	glfwSwapInterval(1);

	this->inputManager = std::make_unique<InputManager>(mainWindow);
      
    Triangle tri = Triangle();
	Grid grid = Grid("Default", 16);
    
    objects.append(tri);
    objects.append(grid);

	while (!glfwWindowShouldClose(mainWindow))
	{
        loop();
	}
    
	glfwDestroyWindow(mainWindow);
	getLogger()->info("Destroying engine...");

	return 0;

}

void GLFWEngine::loop()
{

	this->time = glfwGetTime();
	this->deltaTime = time - oldTime;
	this->oldTime = time;

	float ratio;
	int width, height;

	glfwGetFramebufferSize(mainWindow, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	
	glClear(GL_COLOR_BUFFER_BIT);
    
    float color = 0x30 / 255.0f;
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for (size_t i = 0; i < objects.getCount(); i++)
	{
        objects[i]->render(deltaTime);
	}

	glfwSwapBuffers(mainWindow);
	glfwPollEvents();

}
