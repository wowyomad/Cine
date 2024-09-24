#pragma once
#include "glash/glash_pch.hpp"

#include <GLFW/glfw3.h>
#include "glash/Enums.hpp"


namespace glash
{
    class Window
    {

    public:
        Window(int width, int height, const std::string& title, const Color& clearColor = color::GRAY);
        ~Window();

        void PollEvents() const;
        bool ShouldClose() const;
        void ClearBuffer();
        void SwapBuffers() const;

        GLFWwindow* GetPtr();
        std::string GetGLVersion() const;

    public:
        void GetWindowSize(int* w, int* h) const;
        void SetKeyCallback(GLFWkeyfun callback);


        void SetClearColor(const Color& color);
        Color GetClearColor() const;
    protected:
        GLFWwindow* m_pWindow;
        Color m_ClearColor;
    };
} //namespace glash
