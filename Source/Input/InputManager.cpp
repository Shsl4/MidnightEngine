#include <Input/InputManager.h>
#include <exception>

InputManager::InputManager(GLFWwindow* window)
{
    if(instance) { throw new std::runtime_error("There should not be more that one instance of InputManager."); }
    this->logger = std::make_unique<Logger>("InputManager");
    glfwSetKeyCallback(window, onInput);
    instance = this;
}

void InputManager::onInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /**
     * Mods: 1 = Shift, 2 = Control, 4 = Alt, 8 = Windows
     */
    instance->logger->debug("Key: {}, Scancode: {}, Action: {}, Mods: {}", key, scancode, action, mods);

    if (key == 300 && action == 1) {
        GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);
        
        if (glfwGetWindowMonitor(window)) {
            glfwSetWindowMonitor(window, nullptr, 200, 200, 500, 500, GLFW_DONT_CARE);
        }
        else {
            glfwSetWindowMonitor(window, mainMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }

    if (key == 256) {
        glfwSetWindowShouldClose(window, true);
    }

}
