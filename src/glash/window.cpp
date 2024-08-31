<<<<<<< Updated upstream
=======
#include "glash/glash_pch.hpp"
>>>>>>> Stashed changes
#include "window.hpp"
#include "glash/helper/logger.hpp"
namespace glash
{

    Window::Window(int width, int height, const std::string &title, const Color& clearColor)
        : m_ClearColor(clearColor)
    {
        glash::initLogger();

        LOG_INFO("Window constructor!");


        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_pWindow);

        glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
            {
                glViewport(0, 0, width, height);
            });

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            throw std::runtime_error("Failed to initialize GLEW");
        }

        fmt::println("OpenGL Version: {}\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_pWindow);
    }

    void Window::ClearBuffer()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.y);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(m_pWindow);
    }

    void Window::GetWindowSize(int* w, int* h) const
    {
        glfwGetWindowSize(m_pWindow, w, h);
    }

    void Window::SetKeyCallback(GLFWkeyfun callback)
    {
        glfwSetKeyCallback(m_pWindow, callback);
    }

    void Window::SetClearColor(const Color& color)
    {
        m_ClearColor = color;
    }

    Color Window::GetClearColor() const
    {
        return m_ClearColor;
    }

} // namespace glash
