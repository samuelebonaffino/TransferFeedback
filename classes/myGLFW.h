#ifndef MY_GLFW
#define MY_GLFW

#include "../lib/glad.h"

#include <iostream>
#include <GLFW/glfw3.h>

class MyGLFW
{
    private:
        GLFWwindow* window;

    public:
    MyGLFW(int major, int minor, int width, int height, const char* windowTitle)
    {
        // GLFW: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // GLFW: create window
        window = createWindow(width, height, windowTitle);
        if(window == NULL)
            std::cout << "Failed to create window" << std::endl;
        glfwMakeContextCurrent(window);

        // GLAD: load all OpenGL function pointers
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            std::cout << "Failed to initialize GLAD" << std::endl;
    }

    GLFWwindow* getWindow()
    {
        return window;
    }
    GLFWwindow* createWindow(uint width, uint height, const char* windowTitle)
    {
        GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
        if(window == NULL)
        {
            std::cout << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return NULL;
        }
        return window;
    }
    void processInput()
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    void terminate()
    {
        glfwTerminate();
    }
};

#endif