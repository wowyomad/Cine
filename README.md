# glash

## Project Overview
OpenGL + C++ + CMake "eNgInE" for eductional purposes

## Library Dependencies
This project relies on the following external libraries, which are fetched, built and linked automatically using CMake's `FetchContent` module:

1. **GLEW (OpenGL Extension Wrangler Library)**
   - **Repository:** [glew-cmake](https://github.com/Perlmint/glew-cmake.git)
   - **Build Configuration:** Built as a static library.

2. **GLFW (Graphics Library Framework)**
   - **Repository:** [glfw](https://github.com/glfw/glfw.git)
   - **Build Configuration:** Built as a static library

3. **fmt (Formatting Library)**
   - **Repository:** [fmtlib](https://github.com/fmtlib/fmt.git)

4. **GLM (OpenGL Mathematics)**
   - **Repository:** [glm](https://github.com/g-truc/glm.git

5. **OpenGL**
   - **Required Package:** OpenGL (automatically found by CMake)
  
## Options
- **GLASH_BUILD_SHARED:** Controls whether `glash` is built as a shared library (default: OFF).
- **GLASH_BUILD_EXAMPLES:** Enables building of example projects (default: ON).

## Instructions
Ensure that you have CMake (minimum version 3.20) installed on your system. To build the project, clone the repository and run the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
