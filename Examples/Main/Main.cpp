#include <Coel.hpp>
#include <iostream>

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;

uniform mat4 viewMat;

void main() {
	gl_Position = viewMat * vec4(pos, 0, 1);
}
)";
static constexpr const char *const fragSrc = R"(
#version 440

out vec4 color;

void main() {    
    color = vec4(0.1, 0.1, 0.1, 1);
}
)";

Coel::Window window(1400, 900, "Main");

Coel::Renderer::Batch2d renderer;
Coel::Shader shader(vertSrc, fragSrc);

Math::Mat4 viewMat;

int main() {
    window.onResize = [](Coel::Window &w) {
        viewMat = Math::Mat4::ortho(0, (float)w.size.x, (float)w.size.y, 0, -1, 1);
        shader.sendMat4("viewMat", &viewMat);
    };

    window.onMouseButton = [](Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
            switch (w.key.code) {
            case Coel::Key::W:
                std::cout << "W key pressed\n"; //
                break;
            default: break;
            }
            break;
        default: break;
        }
    };

    window.onResize(window);
    Coel::Renderer::Command::setClearColor(0.2f, 0.4f, 0.8f, 1.f);

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();
        shader.bind();

        renderer.submitRect(20, 20, 100, 100);

        renderer.flush();
        window.update();
    }
}
