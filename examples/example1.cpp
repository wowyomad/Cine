#include "glash/shader.hpp"
#include "glash/window.hpp"

#include "fmt/format.h"
#include <iostream>

using namespace glash;

int main(int argc, char** argv)
{
	GLuint shader;
	try {
		glash::Window window(800, 600, "Hello World");

		shader = shader::MakeShader("shaders/vertex.vert", "shaders/fragment.frag");
		if (shader > 0)
		{
			window.SetClearColor(glash::Color::GREEN());
		}
		else 
		{
			window.SetClearColor(glash::Color::RED());
		}

		while (!window.ShouldClose()) {
			window.PollEvents();
			glUseProgram(shader);
			window.UpdateBuffer();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	std::cerr << "Cloing..." << std::endl;

	glDeleteProgram(shader);
	return 0;
}
