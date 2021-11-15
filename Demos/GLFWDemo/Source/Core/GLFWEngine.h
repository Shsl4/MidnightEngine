#pragma once

#include <Core/IMidnightEngine.h>
#include <Core/EngineTypes.h>
#include <Core/ErrorHandler.h>
#include <Core/Logging/Logger.h>
#include <Rendering/ShaderProgram.h>
#include <Utilities/FileHelper.h>
#include <Input/InputManager.h>
#include <Math/MathUtils.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Rendering/RenderObject.h>
#include <Rendering/Triangle.h>
#include <Memory/Array.h>
#include <vector>

class GLFWEngine : public IMidnightEngine
{
public:

    GLFWEngine() : IMidnightEngine() {

    }
    
    virtual int init(int argc, const char** argv) override;

    virtual void loop() override;

    virtual FORCEINLINE double getDeltaTime() const override { return this->deltaTime; }
    virtual FORCEINLINE double getTime() const override { return this->time; }
    virtual FORCEINLINE GLFWwindow* getMainWindow() { return this->mainWindow; }

private:

    double deltaTime = 0.0f;
    double time = 0.0f;
    double oldTime = 0.0f;

    GLFWwindow* mainWindow = nullptr;

    UniquePtr<InputManager> inputManager;
    Array<RenderObject> objects;

    unsigned int linesBufferId = 0;
    
};
