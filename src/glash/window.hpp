#pragma once

#include <string>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace glash
{
    class Window
    {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void PollEvents();
        bool ShouldClose() const;
        void SwapBuffers();
    private:
        GLFWwindow* m_pWindow;
    };
} //namespace glash