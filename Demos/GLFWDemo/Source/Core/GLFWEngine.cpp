#ifdef _WIN64
#include <Windows.h>
#endif

#include <Core/GLFWEngine.h>
#include <Rendering/Vertex.h>
#include <cmath>

Vertex triangle[3] =
{

	Vertex(-0.6f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f),
	Vertex(0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
	Vertex(0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)

};

Vertex square[4] =
{

	//Top Right
	Vertex(0.5f, 0.5f, 0.0f, LinearColors::black),
	//Top Left
	Vertex(-0.5f, 0.5f, 1.0f, LinearColors::black),
	//Bottom Right
	Vertex(0.5f, -0.5f, 0.0f, LinearColors::black),
	//Bottom Left
	Vertex(-0.5f, -0.5f, 0.0f, LinearColors::black)

};

GLuint elements[] = {
	0, 1, 2,
	2, 3, 0
};

int GLFWEngine::init(int argc, char** argv)
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_DECORATED, false);

#ifdef FULLSCREEN

	GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);
	mainWindow = glfwCreateWindow(mode->width, mode->height, "MidnightEngine", glfwGetPrimaryMonitor(), nullptr);

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
	
	glGenBuffers(1, &elementBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	ShaderProgram prog = ShaderProgram("Default");
	programs.push_back(prog);

	while (!glfwWindowShouldClose(mainWindow))
	{
		loop();
	}

	glDeleteBuffers(1, &this->elementBufferId);
	glDeleteBuffers(1, &this->vertexBufferId);
    
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

	for (size_t i = 0; i < programs.size(); i++)
	{
		
		Matrix4 id = Matrix4::identity();
		id.rotateZ((float)time);
		Matrix4 matrix = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f) * id;
		programs[i].bind();
		int mvp_location = glGetUniformLocation(programs[i].getProgram(), "viewMatrix");
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)matrix.data);
		
		
		for (size_t j = 0; j < 4; ++j) {
			square[j].color.setRed(abs((float)sin(j % 3 + time + 3.14f / 4.0f)));
			square[j].color.setGreen(abs((float)cos(j % 3 + time)));
			square[j].color.setBlue(abs((float)sin(j % 3 + time - 3.14f)));
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(mainWindow);
	glfwPollEvents();

}
