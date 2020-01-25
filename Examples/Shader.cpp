#include <Coel.hpp>

int main() {
    auto window = Coel::createWindow(800, 600, "Simple Shader Example");

    const char *const vertSrc = R"(
    #version 440

    layout (location = 0) in vec2 pos;

    void main() {
	    gl_Position = vec4(pos / 2, 0, 1);
    })",
                      *const fragSrc = R"(
    #version 440

    out vec4 color;

    void main() {    
        color = vec4(1, 0, 1, 1);
    })";

    struct Vertex {
        Math::Vec2 pos;
    };
    Vertex vertices[] = {
        {-1, -1},
        {1, -1},
        {-1, 1},
        {1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);
    auto shader = Coel::createShader(vertSrc, fragSrc);

    while (!window->shouldClose()) {
        Coel::Renderer::clear();

        vao->draw(ibo);
        window->update();
    }
}
