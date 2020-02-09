
link_libraries(Coelengine)

add_executable(Main Main/Main.cpp)

add_executable(Window Window/Main.cpp)

add_executable(StaticVbo Vbo/Static.cpp)
add_executable(DynamicVbo Vbo/Dynamic.cpp)

add_executable(StaticIbo Ibo/Static.cpp)
add_executable(DynamicIbo Ibo/Dynamic.cpp)

add_executable(Batch2dRenderer Renderer/Batch2d.cpp)

add_executable(Particles Misc/Particles.cpp)
