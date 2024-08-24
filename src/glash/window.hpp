#pragma once

#include "structures.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>


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
    private:
        GLFWwindow* m_pWindow;
        Color m_ClearColor = Color::GREEN();
    };
} //namespace glash