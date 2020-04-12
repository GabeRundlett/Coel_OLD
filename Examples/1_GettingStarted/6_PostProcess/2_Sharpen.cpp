#include "0_Common/Scene.hpp"
#include "0_Common/Shaders/Simple.hpp"

int main() {
    Coel::Window window("Sharpen Filter Example");
    Coel::create(window);
    Scene::init(Simple::vertSrc, Simple::fragSrc);

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
        vec2 off = vec2(1) / textureSize(u_fboTex, 0);
        color = vec4(0);

        color -= texture(u_fboTex, v_tex + vec2(off.x, 0));
        color -= texture(u_fboTex, v_tex + vec2(off.x, off.y));
        color -= texture(u_fboTex, v_tex + vec2(0, off.y));

        color -= texture(u_fboTex, v_tex + vec2(off.x, -off.y));
        color += 9 * texture(u_fboTex, v_tex);
        color -= texture(u_fboTex, v_tex - vec2(off.x, -off.y));
        
        color -= texture(u_fboTex, v_tex - vec2(off.x, 0));
        color -= texture(u_fboTex, v_tex - vec2(off.x, off.y));
        color -= texture(u_fboTex, v_tex - vec2(0, off.y));
        
        color.w = 1;
    })";

    Coel::Fbo fbo;
    Coel::Shader quadShader;
    Coel::Texture fboColTex;
    Coel::Rbo fboDepthRbo;
    Coel::Renderer::Quad2d quadRenderer;

    Coel::create(quadShader, quadVertSrc, quadFragSrc);
    auto u_fboTex = Coel::findInt(quadShader, "u_fboTex");
    Coel::create(fboColTex, window.size, Coel::RGBA, nullptr);
    Coel::create(fboDepthRbo, window.size);
    Coel::create(fbo, window.size);
    Coel::attach(fbo, {fboColTex});
    Coel::attach(fbo, fboDepthRbo);

    while (window.isOpen) {
        Coel::bind(fbo);
        Scene::draw(window.size);

        Coel::bind(window.fbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();
        Coel::bind(quadShader);
        Coel::send(u_fboTex, 0);
        Coel::bind(fboColTex, 0);
        quadRenderer.draw();

        Coel::update(window);
    }
}
