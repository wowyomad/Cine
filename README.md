# Cine

## Project Overview
**Cine** is an OpenGL + C++ + CMake "eNgInE" for educational purposes.

## Library Dependencies
This project relies on the following external libraries, which are fetched, built, and linked automatically using CMake's `FetchContent` module:

1. **EnTT (Entity-Component-System Library for Game Development)**
   - **Repository:** [EnTT](https://github.com/skypjack/entt.git)
   - **Version:** 3.13.2
   - **Build Configuration:** Header-only library.

2. **glad (Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specifications for multiple languages.)**
   - **Repository:** [glad](https://github.com/Dav1dde/glad.git)
   - **Version:** 2.0.7
   - **Build Configuration:** Built as a static library.

3. **GLFW (Graphics Library Framework)**
   - **Repository:** [glfw](https://github.com/glfw/glfw.git)
   - **Version:** 3.4
   - **Build Configuration:** Built as a static library.

4. **fmt (Formatting Library)**
   - **Repository:** [fmtlib](https://github.com/fmtlib/fmt.git)
   - **Version:** 11.0.2
   - **Build Configuration:** Built as a static library.

5. **GLM (OpenGL Mathematics)**
   - **Repository:** [glm](https://github.com/g-truc/glm.git)
   - **Version:** 1.0.1
   - **Build Configuration:** Header-only library.

6. **spdlog (Logging Library)**
   - **Repository:** [spdlog](https://github.com/gabime/spdlog.git)
   - **Version:** 1.14.1
   - **Build Configuration:** Built as a static library.

7. **stb (Single-file public domain libraries for C/C++)**
   - **Repository:** [stb](https://github.com/nothings/stb.git)
   - **Version:** Latest commit (f75e8d1cad7d90d72ef7a4661f1b994ef78b4e31)
   - **Build Configuration:** Header-only library.

8. **ImGui (Immediate Mode GUI)**
   - **Repository:** [imgui](https://github.com/ocornut/imgui.git)
   - **Version:** 1.91.2
   - **Build Configuration:** Built as a static library.

9. **OpenGL**
   - **Required Package:** OpenGL (automatically found by CMake).

## CMake Options
- **Cine_BUILD_DLL:** Controls whether `Cine` is built as a DLL (dynamic library) (default: OFF). If set to `ON`, you should also define `Cine_IMPORT` in projects that use `Cine` and duplicate this setting in your project to properly link the dynamic library.

- **Cine_BUILD_EXAMPLES:** Enables building of example projects (default: ON).

## Build Instructions
Ensure that you have CMake (minimum version 3.20) installed on your system. To build the project, clone the repository and run the following commands:

```bash
git clone https://github.com/yourusername/Cine.git
cd Cine
mkdir build
cd build
cmake ..
cmake --build .
