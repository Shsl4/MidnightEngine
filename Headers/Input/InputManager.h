﻿#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>

class InputManager
{
public:

    InputManager(GLFWwindow* window);
    
private:
    
    static void onInput(GLFWwindow* window, int key, int scancode, int action, int mods);

    UniquePtr<Logger> logger;
    GLFWwindow* targetWindow;

    inline static InputManager* instance = nullptr;
    
};
