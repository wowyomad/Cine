#include "glash/glash_pch.hpp"
#include "glash/window.hpp"
#include "glash/shader.hpp"
#include "glash/mesh.hpp"

inline void RotateAroundCenter(glm::vec3 positions[3], float angle)
{
    // Calculate the center of the triangle
    glm::vec3 center = (positions[0] + positions[1] + positions[2]) / 3.0f;

    // Create a rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotate each position around the center
    for (int i = 0; i < 3; ++i)
    {
        // Translate the vertex to the origin (relative to the center)
        glm::vec4 translatedPosition = glm::vec4(positions[i] - center, 1.0f);

        // Apply the rotation
        translatedPosition = rotationMatrix * translatedPosition;

        // Translate back to the original position
        positions[i] = glm::vec3(translatedPosition) + center;
    }
}

inline void RunTestWindow()
{
    GLuint shader;
    try {
        glash::Window window(800, 600, "Test Window");

        glm::vec3 positions[3] = {
            glm::vec3(-.5f, -.5f, .0f),
            glm::vec3(.5f, -.5f, .0f),
            glm::vec3(.0f, .5f, .0f)
        };

        glm::vec3 colors[3] = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        };

        shader = glash::shader::MakeShader("shaders/vertex.vert", "shaders/fragment.frag");
        glUseProgram(shader);

        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), nullptr, GL_DYNAMIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)(sizeof(positions)));
        glEnableVertexAttribArray(1);

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

            // Rotate the triangle around its center
            RotateAroundCenter(positions, glm::radians(0.5f));

            // Update the vertex positions in the GPU buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);

            // Draw the triangle
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            window.SwapBuffers();
        }

        // Clean up
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    std::cerr << "Closing..." << std::endl;
    glDeleteProgram(shader);
}
