
link_libraries(Coelengine)

add_executable(Main Main/Main.cpp)

add_executable(Window Window/Main.cpp)

add_executable(StaticVbo Vbo/Static.cpp)
add_executable(DynamicVbo Vbo/Dynamic.cpp)

add_executable(StaticIbo Ibo/Static.cpp)
add_executable(DynamicIbo Ibo/Dynamic.cpp)

add_executable(Batch2dRenderer Renderer/Batch2d.cpp)

add_executable(Particles Misc/Particles.cpp)

add_executable(TexturedQuad TexturedQuad/Main.cpp)
add_executable(SimpleFbo Fbo/Simple.cpp)
add_executable(BlurFbo Fbo/Blur.cpp)
add_executable(GaussianFbo Fbo/Gaussian.cpp)

add_executable(Imgui Imgui/Main.cpp)
