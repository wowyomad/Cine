#include "glash/window.hpp"
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    try {
        glash::Window window(800, 600, "Hello World");

        while (!window.ShouldClose()) {
            window.PollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

            window.SwapBuffers();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    std::cerr << "Cloing..." << std::endl;
    return 0;
}
