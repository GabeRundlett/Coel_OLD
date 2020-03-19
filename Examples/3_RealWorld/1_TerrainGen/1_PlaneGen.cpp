#include "0_Resources/Player.hpp"
#include <Coel.hpp>

Player player;
bool inputMode = true;

int main() {
    Coel::Window window(1400, 800, "Terrain Generation - Part 1: Plane Generation");
    Coel::Renderer::ImGuiRenderer imgui(window);

    window.onResize = [](Coel::Window &w) {
        if (inputMode) {
            player.windowSizeUpdate(w.size);
            w.cursorTo(player.mouseCenter);
        }
    };
    window.cursorMode(Coel::Window::CursorMode::Disabled);
    window.onKey = [](Coel::Window &w) {
        if (w.key.code == Coel::Key::Escape)
            if (w.key.action == Coel::Action::Press) {
                if (inputMode) {
                    w.cursorMode(Coel::Window::CursorMode::Normal);
                    inputMode = false;
                } else {
                    w.cursorMode(Coel::Window::CursorMode::Disabled);
                    inputMode = true;
                }
            }
        if (inputMode) player.keyUpdate(w.key);
    };
    window.onMouseScroll = [](Coel::Window &w) {
        if (inputMode) player.mouseScrollUpdate(w.mouse);
    };
    window.onMouseMove = [](Coel::Window &w) {
        if (inputMode) {
            player.mouseMoveUpdate(w.mouse);
            w.cursorTo(player.mouseCenter);
        }
    };

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    out vec4 frag_color;
    void main() {
        frag_color = vec4(1, 1, 1, 1);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");

    glm::mat4 modlMat{1};

    static constexpr unsigned int SIZE_Y = 20, SIZE_X = 20;
    static constexpr unsigned int MAX_VERTEX_COUNT = SIZE_X * SIZE_Y * 6;

    struct Vertex {
        glm::vec3 pos;
    } * vertices;
    unsigned int vertexCount = 0;
    Coel::Vao vao;
    Coel::Vbo vbo(nullptr, sizeof(Vertex) * MAX_VERTEX_COUNT, {{Coel::Element::F32, 3}});
    vao.add(vbo);
    vbo.open(&vertices);

    auto flush = [&vao, &vbo, &vertices, &vertexCount]() {
        vbo.close();
        vao.draw(vertexCount);
        vbo.open(&vertices);
        vertexCount = 0;
    };
    auto setPlaneXZ = [&vertices, &vertexCount, &flush](const float x, const float y, const float z, const float size) {
        if (vertexCount > MAX_VERTEX_COUNT) flush();
        *vertices = {{x, y, z}};
        ++vertices;
        *vertices = {{x, y, z + size}};
        ++vertices;
        *vertices = {{x + size, y, z}};
        ++vertices;
        *vertices = {{x, y, z + size}};
        ++vertices;
        *vertices = {{x + size, y, z}};
        ++vertices;
        *vertices = {{x + size, y, z + size}};
        ++vertices;
        vertexCount += 6;
    };

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
    auto time = window.getTime(), prevTime = time, elapsed = time;

    window.resize();
    player.setPos({0, 2, 0});
    player.setRot({0, 0, 0});

    while (window.isOpen()) {
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::clear();

        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;
        player.update(elapsed);

        shader.send(u_projMat, &player.cam.projMat);
        shader.send(u_viewMat, &player.cam.viewMat);
        shader.send(u_modlMat, &modlMat);

        for (int yi = 0; yi < SIZE_Y; ++yi) {
            for (int xi = 0; xi < SIZE_X; ++xi) {
                setPlaneXZ(xi - SIZE_X / 2, 0, yi - SIZE_Y / 2, 1);
            }
        }

        flush();

        if (!inputMode) {
            Coel::Renderer::enableDepthTest(false);
            Coel::Renderer::clearDepth();

            imgui.begin();
            ImGui::Begin("Settings");

            ImGui::DragFloat("Sensitivity", &player.mouseSensitivity);

            ImGui::End();
            imgui.end(window);
        }

        window.update();
    }
}
