find_path(STB_INCLUDE_DIRS "stb.h")
find_package(assimp CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(Coelengine_ImguiImpl
    Coel/Renderers/ImGui/ImGuiTextEditor.cpp
    Coel/Renderers/ImGui/imgui_impl_glfw.cpp
    Coel/Renderers/ImGui/imgui_impl_opengl3.cpp
)
target_link_libraries(Coelengine_ImguiImpl PUBLIC glfw glad::glad imgui::imgui glm::glm)

add_library(Coelengine
    Coel/Window.cpp
    Coel/Camera.cpp
    Coel/Model.cpp

    Coel/Renderer/Context.cpp
    Coel/Renderer/Renderer.cpp
    Coel/Renderer/Shader.cpp
    Coel/Renderer/Texture.cpp
    Coel/Renderer/Buffer.cpp
    Coel/Renderer/Batch.cpp

    Coel/Renderers/Quad2d.cpp
    Coel/Renderers/Sprite.cpp
    Coel/Renderers/Batch3d.cpp

    Coel/Renderers/ImGui/ImGuiRenderer.cpp
)

target_link_libraries(Coelengine PUBLIC Coelengine_ImguiImpl assimp::assimp)
target_include_directories(Coelengine PUBLIC ${STB_INCLUDE_DIRS})
target_compile_features(Coelengine PUBLIC cxx_std_20)
target_include_directories(Coelengine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
# if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# target_link_options(Coelengine PUBLIC -Wl,/SUBSYSTEM:WINDOWS,/ENTRY:mainCRTStartup)
# elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# target_link_options(Coelengine PUBLIC /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
# endif()
# endif()
target_link_libraries(Coelengine PUBLIC coel_warnings_target)
