#include "input.h"

#include "engine.h"

#include <iostream>

namespace wake
{
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        W_INPUT.KeyEvent.call((KeyboardInput) key, (InputAction) action);
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        W_INPUT.MouseButtonEvent.call((MouseInput) button, (InputAction) action);
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        W_INPUT.CursorPositionEvent.call(xpos, ypos);
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
        glfwSetMouseButtonCallback(window, &mouse_button_callback);
        glfwSetCursorPosCallback(window, &cursor_position_callback);

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
        glfwSetMouseButtonCallback(window, nullptr);
        glfwSetCursorPosCallback(window, nullptr);

        return true;
    }

    InputAction InputManager::getKey(KeyboardInput key) const
    {
        return (InputAction) glfwGetKey(W_ENGINE.getWindow(), (int) key);
    }

    InputAction InputManager::getMouseButton(MouseInput button) const
    {
        return (InputAction) glfwGetMouseButton(W_ENGINE.getWindow(), (int) button);
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