#pragma once

#include <Core/EngineTypes.h>
#include <Core/ErrorHandler.h>
#include <Core/Logging/Logger.h>
#include <Rendering/ShaderProgram.h>
#include <Utilities/FileHelper.h>
#include <Input/InputManager.h>
#include <Math/MathUtils.h>
#include <vector>

class MidnightEngine
{
public:

    MidnightEngine();
    
    int init(int argc, char** argv);

    void loop();

    FORCEINLINE double getDeltaTime() const { return this->deltaTime; }
    FORCEINLINE double getTime() const { return this->time; }

private:

    double deltaTime = 0.0f;
    double time = 0.0f;
    double oldTime = 0.0f;

    inline static MidnightEngine* instance = nullptr;

    GLFWwindow* mainWindow = nullptr;

    UniquePtr<Logger> logger;
    UniquePtr<InputManager> inputManager;
    std::vector<ShaderProgram> programs;

    unsigned int vertexBufferId;
    unsigned int elementBufferId;
    
};
