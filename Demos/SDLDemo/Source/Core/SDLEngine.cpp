#include <iostream>

#define _USE_MATH_DEFINES


#include "SDLEngine.h"
#include <Rendering/Vertex.h>
#include <cmath>

Vertex triangle[3] =
{

    Vertex(-0.6f, -0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f),
    Vertex(0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
    Vertex(0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)

};

int SDLEngine::init(int argc, char** argv)
{
    int width = 640;
    int height = 480;


    getLogger()->info("Initializing MidnightEngine...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {

        getLogger()->fatal("Failed to initialize SDL! Error: {1} {2} {0}", SDL_GetError(), 5, 3);
        return -1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    mainWindow = SDL_CreateWindow("MidnightEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!mainWindow)
    {
        getLogger()->fatal("Failed to create window!");
        return -1;
    }


    SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

    if (!glContext)
    {
        getLogger()->fatal("Failed to create context!");
        return -1;
    }

    SDL_GL_MakeCurrent(mainWindow, glContext);
    gladLoadGL();
    SDL_GL_SetSwapInterval(1);

    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    program = std::make_unique<ShaderProgram>("Default");

    while (shouldRun) {
        loop();
    }

    getLogger()->info("Destroying engine...");

    glDeleteBuffers(1, &this->vertexBufferId);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();


	return 0;
}

void SDLEngine::loop()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                shouldRun = false;
            }
            break;
        case SDL_QUIT:
            shouldRun = false;
            break;
        default:
            break;
        }

    }

    float ratio;
    int width, height;
    SDL_GL_GetDrawableSize(mainWindow, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    Matrix4 id = Matrix4::identity();
    id.rotateZ((float)SDL_GetTicks() * 0.001);
    Matrix4 matrix = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f) * id;
    program->bind();
    int mvp_location = glGetUniformLocation(program->getProgram(), "viewMatrix");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)matrix.data);


    for (size_t j = 0; j < 3; ++j) {
        triangle[j].color.setRed(abs((float)sin(j % 3 + SDL_GetTicks() * 0.001 + 3.14f / 4.0f)));
        triangle[j].color.setGreen(abs((float)cos(j % 3 + SDL_GetTicks() * 0.001)));
        triangle[j].color.setBlue(abs((float)sin(j % 3 + SDL_GetTicks() * 0.001 - 3.14f)));
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(mainWindow);
}
