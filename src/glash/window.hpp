#pragma once
#include "glash/glash_pch.hpp"
#include <GLFW/glfw3.h>

#include "glash/structures.hpp"

#include <functional>
#include "../../out/build/win32-msvc/_deps/fmt-src/include/fmt/ranges.h"


namespace glash
{
    class Window
    {

    public:
        Window(int width, int height, const std::string& title, const Color& clearColor = color::RED);
        ~Window();

        void PollEvents() const;
        bool ShouldClose() const;
        void ClearBuffer();
        void SwapBuffers() const;

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