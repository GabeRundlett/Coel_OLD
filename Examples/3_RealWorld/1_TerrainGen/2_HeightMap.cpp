#include "0_Resources/Player.hpp"
#include <Coel.hpp>

#include <iostream>

Player player;
bool inputMode = true;

int main() {
    Coel::Window window(1400, 800, "Terrain Generation - Part 2: Height Map");
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
    layout (location = 1) in vec3 a_nrm;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    out vec3 v_nrm;
    void main() {
        v_nrm = normalize(a_nrm);
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_nrm;
    out vec4 frag_color;
    void main() {
        vec3 lightDir = normalize(vec3(0, 1, 1));
        float diffuse = max(dot(lightDir, v_nrm), 0);
        frag_color = vec4(vec3(diffuse), 1);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");

    glm::mat4 modlMat{1};

    static constexpr unsigned int SIZE_Y = 200, SIZE_X = 200;
    static constexpr unsigned int MAX_VERTEX_COUNT = SIZE_X * SIZE_Y * 6;
    float viewScale = 2;

    float noiseValues[SIZE_Y][SIZE_X];
    constexpr unsigned NOISE_SAMPLES = 4;
    const float SCALE = 3.f, ROUGHNESS = 0.3f;
    for (unsigned int yi = 0; yi < SIZE_Y; ++yi) {
        for (unsigned int xi = 0; xi < SIZE_X; ++xi) {
            float val = 0.f, octaveScale = 0.01f, octaveRoughness = 1.f;
            for (unsigned int i = 0; i < NOISE_SAMPLES; ++i) {
                val += glm::simplex(glm::vec2(xi, yi) * octaveScale) * octaveRoughness;
                octaveScale *= SCALE, octaveRoughness *= ROUGHNESS;
            }
            noiseValues[yi][xi] = val / viewScale * 10 - 1.6;
        }
    }

    struct Vertex {
        glm::vec3 pos, nrm;
    } * vertices;
    unsigned int vertexCount = 0;
    Coel::Vao vao;
    Coel::Vbo vbo(nullptr, sizeof(Vertex) * MAX_VERTEX_COUNT, {{Coel::Element::F32, 3}, {Coel::Element::F32, 3}});
    vao.add(vbo);
    vbo.open(&vertices);

    auto flush = [&vao, &vbo, &vertices, &vertexCount]() {
        vbo.close();
        vao.draw(vertexCount);
        vbo.open(&vertices);
        vertexCount = 0;
    };
    auto setPlaneXZ = [&vertices, &vertexCount, &flush](const float x, const float y[4], const float z, const float size) {
        if (vertexCount > MAX_VERTEX_COUNT) flush();
        const glm::vec3 v[]{{0, y[1] - y[0], size}, {0, y[3] - y[2], size}},
            w[]{{size, y[2] - y[0], 0}, {size, y[3] - y[1], 0}};
        const glm::vec3 nrm[]{
            {(v[0].y * w[0].z) - (v[0].z * w[0].y),  //
             (v[0].z * w[0].x) - (v[0].x * w[0].z),  //
             (v[0].x * w[0].y) - (v[0].y * w[0].x)}, //
            {(v[1].y * w[1].z) - (v[1].z * w[1].y),  //
             (v[1].z * w[1].x) - (v[1].x * w[1].z),  //
             (v[1].x * w[1].y) - (v[1].y * w[1].x)}, //
        };
        *vertices = {{x, y[0], z}, nrm[0]};
        ++vertices;
        *vertices = {{x, y[1], z + size}, nrm[0]};
        ++vertices;
        *vertices = {{x + size, y[2], z}, nrm[0]};
        ++vertices;
        *vertices = {{x + size, y[2], z}, nrm[1]};
        ++vertices;
        *vertices = {{x, y[1], z + size}, nrm[1]};
        ++vertices;
        *vertices = {{x + size, y[3], z + size}, nrm[1]};
        ++vertices;
        vertexCount += 6;
    };

    Coel::Renderer::enableCulling(true);
    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
    auto time = window.getTime(), prevTime = time, elapsed = time;

    window.resize();
    player.setPos({-0.5f / viewScale * SIZE_X, 2, -0.5f / viewScale * SIZE_Y});
    player.setRot({0, 0, 0});

    while (window.isOpen()) {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;

        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::clear();

        for (int yi = 0; yi < SIZE_Y; ++yi) {
            for (int xi = 0; xi < SIZE_X; ++xi) {
                int pxi = xi - 1, pyi = yi - 1;
                if (xi == 0) pxi = 0;
                if (yi == 0) pyi = 0;

                const float heights[4] = {
                    noiseValues[pyi][pxi],
                    noiseValues[yi][pxi],
                    noiseValues[pyi][xi],
                    noiseValues[yi][xi],
                };
                setPlaneXZ(xi / viewScale, heights, yi / viewScale, 1.f / viewScale);
            }
        }

        float x = -player.pos.x * viewScale, y = -player.pos.z * viewScale;
        int xi = y, yi = y;
        if (xi < 0)
            xi = 0;
        else if (xi > SIZE_X - 1)
            xi = SIZE_X - 1;
        if (yi < 0)
            yi = 0;
        else if (yi > SIZE_Y - 1)
            yi = SIZE_Y - 1;
        float terrainHeight = noiseValues[yi][xi]; //* (x - xi);
        // terrainHeight += noiseValues[yi][xi + 1] * (1.f - (x - xi));
        // terrainHeight += noiseValues[yi][xi];
        // terrainHeight += noiseValues[yi][xi];

        player.update(elapsed, terrainHeight);

        shader.send(u_projMat, &player.cam.projMat);
        shader.send(u_viewMat, &player.cam.viewMat);
        shader.send(u_modlMat, &modlMat);

        flush();

        if (!inputMode) {
            Coel::Renderer::enableDepthTest(false);
            Coel::Renderer::clearDepth();

            imgui.begin();
            ImGui::Begin("Settings");

            ImGui::DragFloat("Sensitivity", &player.mouseSensitivity, 0.001f);

            ImGui::End();
            imgui.end(window);
        }

        window.update();
    }
}
