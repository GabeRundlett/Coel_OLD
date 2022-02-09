#include "0_Common/Scene.hpp"
#include "0_Common/Shaders/Simple.hpp"

int main() {
    Coel::Window window{"Super Sampling Example"};
    Coel::create(window);

    Scene::init(Simple::vertSrc, Simple::fragSrc, window);

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
    uniform sampler2D u_fboTex;
    void main() {
        color = texture(u_fboTex, v_tex);
        color.w = 1;
    })";

    struct RenderTarget {
        Coel::Fbo fbo;
        Coel::Texture fboColTex;
        Coel::Rbo fboDepthRbo;
    } renderTarget;
    Coel::Shader quadShader;
    Coel::Renderer::Quad2d quadRenderer;

    Coel::create(quadShader, quadVertSrc, quadFragSrc);
    auto u_fboTex = Coel::findInt(quadShader, "u_fboTex");

    window.onFboResize = [&renderTarget](Coel::Window &w) {
        const float scale = 0.1f;
        auto size = glm::ivec2(glm::vec2(w.size) * scale);
        Coel::create(renderTarget.fboColTex, size, Coel::RGBA, nullptr);
        Coel::setMagFilter(renderTarget.fboColTex, Coel::Nearest);
        Coel::setMinFilter(renderTarget.fboColTex, Coel::Nearest);
        Coel::setWrapMode(renderTarget.fboColTex, Coel::ClampToEdge);
        Coel::create(renderTarget.fboDepthRbo, size);
        Coel::create(renderTarget.fbo, size);
        Coel::attach(renderTarget.fbo, {renderTarget.fboColTex});
        Coel::attach(renderTarget.fbo, renderTarget.fboDepthRbo);
    };

    window.onFboResize(window);

    while (window.isOpen) {
        Coel::bind(Scene::shader);
        Coel::bind(renderTarget.fbo);
        Scene::draw(window);

        Coel::bind(window.fbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();
        Coel::bind(quadShader);
        Coel::send(u_fboTex, 0);
        Coel::bind(renderTarget.fboColTex, 0);
        quadRenderer.draw();

        Coel::update(window);
    }
}
