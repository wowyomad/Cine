add_subdirectory(glad)

#imgui
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
 )
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${glfw_SOURCE_DIR}/include)

#imguizmo
add_library(ImGuizmo STATIC ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp)
target_include_directories (ImGuizmo PUBLIC ${imguizmo_SOURCE_DIR})
target_link_libraries(ImGuizmo PUBLIC imgui)
set_target_properties(ImGuizmo PROPERTIES
     MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
     FOLDER "Dependencies"
)