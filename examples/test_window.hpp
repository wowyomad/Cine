#include "glash/glash_pch.hpp"
#include "glash/window.hpp"
#include "glash/shader.hpp"
#include "glash/mesh_triangle.hpp"
#include "glash/mesh_rectangle.hpp"

inline void RunTestWindow()
{
    LOG_INFO("Running test window");
    GLuint shader;
    try {
        glash::Window window(800, 600, "Test Window");

        glm::vec3 positions[4] = {
            glm::vec3(-.5f, -.5f, .0f),
            glm::vec3(.5f, -.5f, .0f),
            glm::vec3(.5f, .5f, .0f),
            glm::vec3(-.5f, .5f, .0f)
        };

        glm::vec3 colors[4] = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        };

        shader = glash::shader::MakeShader("shaders/vertex.vert", "shaders/fragment.frag");
        auto mesh = glash::mesh::RectangleMesh(positions, colors);

        if (shader > 0)
        {
            window.SetClearColor(glash::color::GREEN);
        }
        else
        {
            window.SetClearColor(glash::color::RED);
        }

        while (!window.ShouldClose()) {
            window.PollEvents();
            window.ClearBuffer();
            glUseProgram(shader);
            mesh.draw();
            glUseProgram(0);
            window.SwapBuffers();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cerr << "Closing..." << std::endl;
    glDeleteProgram(shader);
}
