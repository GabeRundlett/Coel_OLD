#include "0_Common/Scene.hpp"
#include "0_Common/Shaders/Deferred.hpp"

int main() {
    Coel::Window window({1280, 960}, "Deferred Rendering Example");
    Coel::create(window);
    Scene::init(Deferred::vertSrc, Deferred::fragSrc);

    const char *const quadVertSrc = R"(
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
    uniform sampler2D u_posTex;
    uniform sampler2D u_nrmTex;
    uniform sampler2D u_colTex;
    void main() {
        vec2 tex = v_tex * 2;
        if (v_tex.x > 0.5) {
            if (v_tex.y > 0.5)
                color = texture(u_nrmTex, tex);
            else
                color = texture(u_posTex, tex);
        } else {
            vec4 col = texture(u_colTex, tex);
            if (v_tex.y > 0.5)
                color = col;
            else
                color = vec4(col.w);
        }
        color.w = 1;
    })";

    Coel::Fbo gbufferFbo;
    Coel::Rbo gbufferDepRbo;
    Coel::Texture gbufferPosTex, gbufferNrmTex, gbufferColTex;
    Coel::Shader quadShader;
    Coel::create(gbufferPosTex, window.size, Coel::RGBA16F, nullptr);
    Coel::create(gbufferNrmTex, window.size, Coel::RGB16F, nullptr);
    Coel::create(gbufferColTex, window.size, Coel::RGBA, nullptr);
    Coel::create(gbufferDepRbo, window.size);
    Coel::create(gbufferFbo, window.size);
    Coel::attach(gbufferFbo, {gbufferPosTex, gbufferNrmTex, gbufferColTex});
    Coel::attach(gbufferFbo, gbufferDepRbo);
    Coel::create(quadShader, quadVertSrc, quadFragSrc);

    auto u_posTex = Coel::findInt(quadShader, "u_posTex");
    auto u_nrmTex = Coel::findInt(quadShader, "u_nrmTex");
    auto u_colTex = Coel::findInt(quadShader, "u_colTex");
    Coel::Renderer::Quad2d quadRenderer;

    while (window.isOpen) {
        Coel::bind(gbufferFbo);
        Scene::draw(window.size);

        Coel::bind(window.fbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();

        Coel::bind(quadShader);
        Coel::send(u_posTex, 0);
        Coel::bind(gbufferPosTex, 0);
        Coel::send(u_nrmTex, 1);
        Coel::bind(gbufferNrmTex, 1);
        Coel::send(u_colTex, 2);
        Coel::bind(gbufferColTex, 2);
        quadRenderer.draw();

        Coel::update(window);
    }
}