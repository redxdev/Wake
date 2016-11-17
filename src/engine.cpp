#include "engine.h"

#include <iostream>
#include <glm/glm.hpp>

namespace wake
{
    static void error_callback(int err, const char* description)
    {
        std::cout << "GLFW error: " << description << std::endl;
    }

    Engine& Engine::get()
    {
        static Engine instance;
        return instance;
    }

    bool Engine::startup()
    {
        glfwSetErrorCallback(&error_callback);
        if (!glfwInit())
        {
            std::cout << "Unable to initialize GLFW." << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(targetWidth, targetHeight, targetTitle,
                                  targetFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Unable to create window." << std::endl;
            glfwTerminate();

            return false;
        }

        glfwMakeContextCurrent(window);

        if (gl3wInit())
        {
            std::cout << "Unable to initialize GL3W." << std::endl;
            glfwTerminate();

            return false;
        }

        W_GL_CHECK();

        if (!gl3wIsSupported(3, 3))
        {
            std::cout << "OpenGL 3.3 not supported on this system" << std::endl;
            glfwTerminate();

            return false;
        }

        glViewport(0, 0, targetWidth, targetHeight);

        setClearColor(0.f, 0.f, 0.f, 1.f);
        clearR = 0.f;
        clearG = 0.f;
        clearB = 0.f;
        clearA = 1.f;

        W_GL_CHECK();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        W_GL_CHECK();

        return true;
    }

    bool Engine::shutdown()
    {
        glfwTerminate();
        window = nullptr;

        return true;
    }

    bool Engine::run()
    {
        if (running)
        {
            std::cout << "Cannot run() while already running!" << std::endl;
            return false;
        }

        if (!window)
        {
            std::cout << "Cannot run engine with uninitialized window (did startup succeed?)" << std::endl;
            return false;
        }

        running = true;

        double lastTime = glfwGetTime();

        while (running && !glfwWindowShouldClose(window))
        {
            double frameTime = glfwGetTime() - lastTime;
            lastTime = glfwGetTime();

            glfwPollEvents();

            int displayW, displayH;
            glfwGetFramebufferSize(window, &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            glClearColor(clearR, clearG, clearB, clearA);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            EarlyTickEvent.call(frameTime);

            TickEvent.call(frameTime);

            LateTickEvent.call(frameTime);

            glfwSwapBuffers(window);
        }

        running = false;

        QuitEvent.call();

        return true;
    }

    bool Engine::isRunning() const
    {
        return running;
    }

    void Engine::stop()
    {
        running = false;
    }

    double Engine::getTime() const
    {
        return glfwGetTime();
    }

    void Engine::setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
    {
        clearR = r;
        clearG = g;
        clearB = b;
        clearA = a;
        glClearColor(r, g, b, a);
    }

    void Engine::setWindowSize(int width, int height)
    {
        targetWidth = width;
        targetHeight = height;

        if (window != nullptr)
        {
            glfwSetWindowSize(window, width, height);
            glViewport(0, 0, width, height);
        }
    }

    void Engine::setWindowFullscreen(bool fullscreen)
    {
        targetFullscreen = fullscreen;
        if (window != nullptr)
        {
            std::cout << "Fullscreen mode switching at runtime is not currently supported" << std::endl;
        }
    }

    void Engine::setWindowTitle(const char* title)
    {
        targetTitle = title;
        if (window != nullptr)
        {
            glfwSetWindowTitle(window, title);
        }
    }

    int Engine::getWindowWidth() const
    {
        int result;
        glfwGetWindowSize(window, &result, NULL);
        return result;
    }

    int Engine::getWindowHeight() const
    {
        int result;
        glfwGetWindowSize(window, NULL, &result);
        return result;
    }

    GLFWwindow* Engine::getWindow() const
    {
        return window;
    }

    Engine::Engine()
    {

    }

    Engine::Engine(const Engine& other)
    {

    }

    Engine& Engine::operator=(const Engine& other)
    {
        return *this;
    }

    Engine::~Engine()
    {

    }
}
