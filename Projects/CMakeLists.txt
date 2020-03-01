
link_libraries(Coelengine)

add_executable(Editor Editor/Main.cpp)
add_executable(Physics Physics/Main.cpp)
add_executable(Tiles Tiles/Main.cpp)
add_executable(TextTest Text/Main.cpp)
add_executable(Raymarch Raymarch/Main.cpp)
add_executable(Minecraft 
    Minecraft/Main.cpp 
    Minecraft/Game.cpp
    Minecraft/Renderer.cpp
    Minecraft/Player.cpp
    Minecraft/Chunk.cpp
    Minecraft/Mesh.cpp
)
