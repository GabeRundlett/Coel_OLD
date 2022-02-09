#include <Coel.hpp>
// #include <unistd.h>

int main() {
    Coel::Window window{{1024, 1024}, "Cellular Automata"};
    Coel::create(window);

    const char *const vertSrc = R"(
#version 440
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex;
out vec2 v_tex;
void main() {
    v_tex = a_tex;
    gl_Position = vec4(a_pos, 0, 1);
})";

    const char *const quadFragSrc = R"(
#version 440
in vec2 v_tex;
out vec4 color;
uniform sampler2D u_tex;
void main() {
    vec4 tex = texture(u_tex, v_tex);
    color = vec4(tex.r);
})";

    const char *const startGridFragSrc = R"(
#version 440
in vec2 v_tex;
out vec4 color;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

void main() {
    float r = random(v_tex);
    if (r > 0.9) color = vec4(1);
    else color = vec4(0);
})";

    const char *const automataVertSrc = R"(
#version 440
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex;
out vec2 v_tex[9];
uniform vec2 tileSize;
void main() {
    gl_Position = vec4(a_pos, 0, 1);
    for (int yi = -1; yi < 2; yi++) {
        for (int xi = -1; xi < 2; xi++) {
            const int index = (xi + 1) + (yi + 1) * 3;
            v_tex[index] = a_tex + vec2(xi, yi) * tileSize;
        }   
    }
})";

    const char *const automataFragSrc = R"(
#version 440
in vec2 v_tex[9];
out vec4 color;
uniform sampler2D u_tex;
void main() {
    int kernel = 0;
    int neighbor_count = 0;
    bool alive = false;
    for (int i = 0; i < 9; i++) {
        vec4 result = texture(u_tex, v_tex[i]);
        if (result.r > 0){
            kernel |= 1 << i;
            if (i == 4) 
                alive = true;
            else
                neighbor_count++;
        }
    }

    if (alive) {
        if (neighbor_count < 2) 
            color = vec4(0);
        else if (neighbor_count > 3) 
            color = vec4(0);
        else 
            color = vec4(1);
    } else {
        if (neighbor_count == 3)
            color = vec4(1);
        else
            color = vec4(0);
    }
})";

    Coel::Renderer::Quad2d quadRenderer;
    Coel::Shader quadShader;
    Coel::create(quadShader, vertSrc, quadFragSrc);
    auto u_tex = Coel::findInt(quadShader, "u_tex");

    constexpr glm::ivec2 gridSize = {256, 256};
    glm::vec2 tileSize = {1.f / gridSize.x, 1.f / gridSize.y};

    Coel::Shader startGridShader;
    Coel::create(startGridShader, vertSrc, startGridFragSrc);
    // auto u_copyGridTex = Coel::findInt(startGridShader, "u_tex");

    Coel::Shader automataShader;
    Coel::create(automataShader, automataVertSrc, automataFragSrc);
    auto u_automataTex = Coel::findInt(automataShader, "u_tex");
    auto u_tileSize = Coel::findFloat2(automataShader, "tileSize");

    Coel::Texture gridTextures[2];
    Coel::create(gridTextures[0], gridSize, Coel::RED, nullptr);
    Coel::create(gridTextures[1], gridSize, Coel::RED, nullptr);

    for (auto &tex : gridTextures) {
        Coel::setMinFilter(tex, Coel::Nearest);
        Coel::setMagFilter(tex, Coel::Nearest);
    }

    Coel::Rbo fboDepthRbo;
    Coel::create(fboDepthRbo, gridSize);

    int i = 0;
    Coel::Fbo gridFbos[2];
    for (auto &fbo : gridFbos) {
        Coel::create(fbo, gridSize);
        Coel::attach(fbo, {gridTextures[i]});
        Coel::attach(fbo, fboDepthRbo);
        ++i;
    }

    Coel::bind(startGridShader);
    Coel::bind(gridFbos[0]);
    quadRenderer.draw();
    Coel::bind(gridFbos[1]);
    quadRenderer.draw();

    const auto automata_from_to = [&](int i1, int i2) {
        Coel::bind(automataShader);
        Coel::bind(gridFbos[i2]);
        Coel::send(u_automataTex, 0);
        Coel::bind(gridTextures[i1], 0);
        Coel::send(u_tileSize, &tileSize);
        Coel::Renderer::setClearColor(0, 0, 0, 1);
        Coel::Renderer::clearColor();
        quadRenderer.draw();
    };

    i = 0;
    const auto run_automata = [&] {
        automata_from_to(i, !i);
        i = !i;
    };

    window.onKey = [&](Coel::Window &w) {
        if (w.key.action != Coel::Action::Release)
            run_automata();
    };

    while (window.isOpen) {
        // run_automata();

        Coel::bind(window.fbo);
        Coel::Renderer::setClearColor(0.5f, 0.0f, 0.9f, 1.0f);
        Coel::Renderer::clearColor();
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::enableBlend(true);
        Coel::bind(quadShader);
        Coel::send(u_tex, 0);
        Coel::bind(gridTextures[i], 0);
        quadRenderer.draw();

        // usleep(10000);
        Coel::update(window);
    }
}
