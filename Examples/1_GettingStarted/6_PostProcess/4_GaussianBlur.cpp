#include "0_Common/Scene.hpp"
#include "0_Common/Shaders/Simple.hpp"

int main() {
    Coel::Window window("Blurring Filter Example");
    Coel::create(window);
    Scene::init(Simple::vertSrc, Simple::fragSrc);

    const char *const gaussVertSrc[2]{
        R"(
    #version 440
    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec2 tex;
    out vec2 vTex[11];
    uniform float u_renderTargetSize;
    void main() {
        gl_Position = vec4(pos, 0, 1);
        float pixelWidth = 1.f / u_renderTargetSize;
        for (int i = -5; i < 5; ++i) {
            vTex[i + 5] = tex + vec2(pixelWidth * i, 0);
        }
    })",
        R"(
    #version 440
    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec2 tex;
    out vec2 vTex[11];
    uniform float u_renderTargetSize = 100;
    void main() {
        gl_Position = vec4(pos, 0, 1);
        float pixelHeight = 1.f / u_renderTargetSize;
        for (int i = -5; i < 5; ++i) {
            vTex[i + 5] = tex + vec2(0, pixelHeight * i);
        }
    })"};

    const char *const gaussFragSrc = R"(
    #version 440
    in vec2 vTex[11];
    out vec4 color;
    uniform sampler2D u_tex;
    const float gammaPower = 2.2;
    vec4 gammaUndistort(in vec4 val) { return pow(val, vec4(gammaPower)); }
    vec4 gammaRedistort(in vec4 val) { return pow(val, vec4(1.0/gammaPower)); }
    void main() {
        color = vec4(0);
        color += gammaUndistort(texture(u_tex, vTex[0])) * 0.0093;
        color += gammaUndistort(texture(u_tex, vTex[1])) * 0.028002;
        color += gammaUndistort(texture(u_tex, vTex[2])) * 0.065984;
        color += gammaUndistort(texture(u_tex, vTex[3])) * 0.121703;
        color += gammaUndistort(texture(u_tex, vTex[4])) * 0.175713;
        color += gammaUndistort(texture(u_tex, vTex[5])) * 0.198596;
        color += gammaUndistort(texture(u_tex, vTex[6])) * 0.175713;
        color += gammaUndistort(texture(u_tex, vTex[7])) * 0.121703;
        color += gammaUndistort(texture(u_tex, vTex[8])) * 0.065984;
        color += gammaUndistort(texture(u_tex, vTex[9])) * 0.028002;
        color += gammaUndistort(texture(u_tex, vTex[10])) * 0.0093;
        color = gammaRedistort(color);
        color.w = 1;
    })";

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
    uniform sampler2D u_blurredTex;
    void main() {
        vec4 blurCol = texture(u_blurredTex, v_tex);
        color = blurCol;
        color.w = 1;
    })";

    Coel::Fbo gaussFbos[4];
    Coel::Rbo gaussFboRbos[4];
    Coel::Texture gaussFboTexs[4];
    Coel::Shader gaussShader[2];
    Coel::Uniform<int> u_gaussTex[2];
    Coel::Uniform<float> u_renderTargetSize[2];

    for (int i = 0; i < 4; ++i) {
        const int scale = (i % 2 + 1) * 2;
        const auto size = window.size / scale;
        Coel::create(gaussFboTexs[i], size, Coel::RGBA, nullptr);
        Coel::setMagFilter(gaussFboTexs[i], Coel::Linear);
        Coel::setMinFilter(gaussFboTexs[i], Coel::Linear);
        Coel::setWrapMode(gaussFboTexs[i], Coel::ClampToEdge);
        Coel::create(gaussFboRbos[i], size);
        Coel::create(gaussFbos[i], size);
        Coel::attach(gaussFbos[i], {gaussFboTexs[i]});
        Coel::attach(gaussFbos[i], gaussFboRbos[i]);
    }

    for (int i = 0; i < 2; ++i) {
        Coel::create(gaussShader[i], gaussVertSrc[i], gaussFragSrc);
        u_gaussTex[i] = Coel::findInt(gaussShader[i], "u_tex");
        u_renderTargetSize[i] = Coel::findFloat(gaussShader[i], "u_renderTargetSize");
    }

    Coel::Fbo fbo;
    Coel::Shader quadShader;
    Coel::Texture fboColTex;
    Coel::Rbo fboDepthRbo;
    Coel::Renderer::Quad2d quadRenderer;
    Coel::create(quadShader, quadVertSrc, quadFragSrc);
    auto u_blurredTex = Coel::findInt(quadShader, "u_blurredTex");
    Coel::create(fboColTex, window.size, Coel::RGBA, nullptr);
    Coel::setMagFilter(fboColTex, Coel::Linear);
    Coel::setMinFilter(fboColTex, Coel::Linear);
    Coel::setWrapMode(fboColTex, Coel::ClampToEdge);
    Coel::create(fboDepthRbo, window.size);
    Coel::create(fbo, window.size);
    Coel::attach(fbo, {fboColTex});
    Coel::attach(fbo, fboDepthRbo);

    while (window.isOpen) {
        Coel::bind(Scene::shader);
        Coel::bind(fbo);
        Scene::draw(window.size);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::enableDepthTest(false);

        for (int i = 0; i < 4; ++i) {
            const int id = i % 2;
            Coel::bind(gaussFbos[i]);
            Coel::Renderer::clearColor();
            Coel::bind(gaussShader[id]);
            Coel::send(u_gaussTex[id], 0);
            if (i == 0)
                Coel::bind(fboColTex, 0);
            else
                Coel::bind(gaussFboTexs[i - 1], 0);
            Coel::send(u_renderTargetSize[id], gaussFbos[i].size[id]);
            quadRenderer.draw();
        }

        Coel::bind(window.fbo);
        Coel::Renderer::clearColor();
        Coel::bind(quadShader);
        Coel::send(u_blurredTex, 0);
        Coel::bind(gaussFboTexs[1], 0);
        quadRenderer.draw();

        Coel::update(window);
    }
}
