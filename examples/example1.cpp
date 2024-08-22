#include "glash/window.hpp"
#include <iostream>
#include <array>
#include <chrono>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <fmt/format.h>
#include <thread>



class ColorGenerator
{
	using Color = glash::Color;

public:
	ColorGenerator()
		: colors{ Color::RED(), Color::GREEN(), Color::BLUE() }, index(0) {}

	Color NextColor()
	{
		Color color = colors[index];

		auto now = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - previous) > m_TimeOut)
		{
			index = (index + 1) % colors.size();
			previous = now;
		}


		return color;
	}

private:
	std::array<Color, 3> colors;
	size_t index = 0;
	std::chrono::milliseconds m_TimeOut = std::chrono::milliseconds(1000);
	std::chrono::high_resolution_clock::time_point previous;
};

void vec_sum()
{
	while (true)
	{
		glm::vec2 vec1;
		std::cout << "x1: "; std::cin >> vec1.x;

		std::cout << "y1: "; std::cin >> vec1.y;

		glm::vec2 vec2;
		std::cout << "x1: "; std::cin >> vec2.x;
		std::cout << "y1: "; std::cin >> vec2.y;

		glm::vec2 sum = vec1 + vec2;
		fmt::println("Sum: {}, {}", sum.x, sum.y);
	}
}



int main(int argc, char** argv) {
	std::cout << "Hello, World!" << std::endl;
	try {
		glash::Window window(800, 600, "Hello World");
		auto generator = ColorGenerator();

		std::jthread test_thread = std::jthread(vec_sum);

	
		while (!window.ShouldClose()) {
			window.PollEvents();
			window.SetClearColor(generator.NextColor());

			window.UpdateBuffer();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	std::cerr << "Cloing..." << std::endl;
	return 0;
}
