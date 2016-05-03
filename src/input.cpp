#include "input.h"

#include "engine.h"

#include <iostream>

namespace wake
{
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        W_INPUT.KeyEvent.call((KeyboardInput) key, (InputAction) action);
    }

    InputManager& InputManager::get()
    {
        static InputManager instance;
        return instance;
    }

    bool InputManager::startup()
    {
        GLFWwindow* window = W_ENGINE.getWindow();
        if (window == nullptr)
        {
            std::cout << "InputManager cannot be initialized without a valid window from the engine." << std::endl;
            return false;
        }

        glfwSetKeyCallback(window, &key_callback);

        return true;
    }

    bool InputManager::shutdown()
    {
        GLFWwindow* window = W_ENGINE.getWindow();
        if (window == nullptr)
        {
            std::cout << "InputManager cannot be shutdown without a valid window from the engine." << std::endl;
            return false;
        }

        glfwSetKeyCallback(window, nullptr);

        return true;
    }

    InputAction InputManager::getKey(KeyboardInput key) const
    {
        return (InputAction) glfwGetKey(W_ENGINE.getWindow(), (int) key);
    }

    InputManager::InputManager()
    {

    }

    InputManager::InputManager(const InputManager& other)
    {

    }

    InputManager& InputManager::operator=(const InputManager& other)
    {
        return *this;
    }

    InputManager::~InputManager()
    {

    }
}