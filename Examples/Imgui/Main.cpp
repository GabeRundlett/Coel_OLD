#include <Coel.hpp>

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

uniform vec4 uCol;
out vec4 color;

void main() {
	color = uCol;
}
)";

int main() {
    Coel::Window window(1280, 960, "Window");
    Coel::Renderer::ImGuiRenderer imgui(window);

    glm::vec4 color{1, 0, 0, 1};
    Coel::Shader quadShader(vertSrc, fragSrc);
    Coel::Renderer::Quad2d quad;

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.sendFloat4("uCol", &color);
        quad.draw();

        imgui.begin();
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Color", (float *)&color);
        ImGui::End();
        imgui.end(window);

        window.update();
    }
}
