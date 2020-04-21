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
    uniform sampler2D u_depTex;
    void main() {
        vec4 pos = texture(u_posTex, v_tex);
        vec4 nrm = texture(u_nrmTex, v_tex);
        vec4 col = texture(u_colTex, v_tex);
        vec4 dep = texture(u_depTex, v_tex);

        color = dep;
    })";

    const char *const depthVertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec4 v_pos;
    out vec3 v_nrm;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        v_pos = u_modlMat * vec4(a_pos, 1);
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * v_pos;
    })";

    const char *const depthFragSrc = R"(
    #version 440
    void main() {
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

    Coel::Fbo depthFbo;
    Coel::Texture depthTex;
    Coel::Shader depthShader;
    Coel::create(depthTex, window.size, Coel::Depth, nullptr);
    Coel::create(depthFbo, window.size);
    Coel::attach(depthFbo, {depthTex});
    Coel::create(depthShader, depthVertSrc, depthFragSrc);

    auto u_posTex = Coel::findInt(quadShader, "u_posTex");
    auto u_nrmTex = Coel::findInt(quadShader, "u_nrmTex");
    auto u_colTex = Coel::findInt(quadShader, "u_colTex");
    auto u_depTex = Coel::findInt(quadShader, "u_depTex");
    Coel::Renderer::Quad2d quadRenderer;

    while (window.isOpen) {
        Coel::bind(Scene::shader);
        Coel::bind(gbufferFbo);
        Scene::draw(window.size);

        Coel::bind(depthShader);
        Coel::bind(gbufferFbo);
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::clearDepth();
        Scene::draw(window.size);

        Coel::bind(window.fbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);

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
