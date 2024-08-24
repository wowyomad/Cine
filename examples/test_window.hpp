#include "glash_pch.hpp"

#include "glash/window.hpp"
#include "glash/shader.hpp"

// Will turn green if all done successfully, otherwise red.
inline void RunTestWindow()
{
	GLuint shader;
	try {
		glash::Window window(800, 600, "Test Window");

		shader = glash::shader::MakeShader("shaders/vertex.vert", "shaders/fragment.frag");
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
			glUseProgram(shader);
			window.UpdateBuffer();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	std::cerr << "Cloing..." << std::endl;
	glDeleteProgram(shader);
}


