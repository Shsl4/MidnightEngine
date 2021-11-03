#pragma once

#define SDL_MAIN_HANDLED

#include <Core/IMidnightEngine.h>
#include <Core/EngineTypes.h>
#include <Core/ErrorHandler.h>
#include <Core/Logging/Logger.h>
#include <Rendering/ShaderProgram.h>
#include <Math/MathUtils.h>
#include <glad/glad.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


class SDLEngine : public IMidnightEngine {

public:

    virtual int init(int argc, char** argv) override;
    virtual void loop() override;

    virtual FORCEINLINE double getDeltaTime() const override { return 0.0f; }
    virtual FORCEINLINE double getTime() const override { return 0.0f; }

private:

    bool shouldRun = true;
    double deltaTime = 0.0f;
    double time = 0.0f;
    double oldTime = 0.0f;

    SDL_Window* mainWindow = nullptr;

    UniquePtr<ShaderProgram> program = nullptr;

    unsigned int vertexBufferId = 0;
};
