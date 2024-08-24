#pragma once
#include "glash_pch.hpp"
#include <GLFW/glfw3.h>

#include "glash/structures.hpp"


namespace glash
{
    class Window
    {

    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void PollEvents() const;
        bool ShouldClose() const;
        void UpdateBuffer();


        void SetClearColor(const Color& color);
        Color GetClearColor() const;
    protected:
        GLFWwindow* m_pWindow;
        Color m_ClearColor;
    };
} //namespace glash