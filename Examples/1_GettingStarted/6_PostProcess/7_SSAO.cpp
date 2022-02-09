#include "0_Common/Shaders/Deferred.hpp"
#include "0_Common/Scene.hpp"

#include "0_Common/Ssao.hpp"

int main() {
    Coel::Window window({1280, 960}, "SSAO Example");
    Coel::create(window);

    Scene::init(Deferred::vertSrc, Deferred::fragSrc, window);

    const char *const quadVertSrc = R"(
    #version 450
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    })";

    const char *const quadFragSrc = R"(
    #version 450
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_posTex;
    uniform sampler2D u_nrmTex;
    uniform sampler2D u_colTex;
    uniform sampler2D u_ssaoTex;
    void main() {
        vec3 pos = texture(u_posTex, v_tex).xyz;
        vec3 nrm = normalize(texture(u_nrmTex, v_tex).xyz);
        vec3 col = texture(u_colTex, v_tex).rgb;
        float ao = texture(u_ssaoTex, v_tex).r;
        color = vec4(col, 1);
        color.rgb *= vec3(ao);
    })";

    Coel::Shader quadShader;
    Coel::create(quadShader, quadVertSrc, quadFragSrc);
    auto u_quad_posTex = Coel::findInt(quadShader, "u_posTex");
    auto u_quad_nrmTex = Coel::findInt(quadShader, "u_nrmTex");
    auto u_quad_colTex = Coel::findInt(quadShader, "u_colTex");
    auto u_quad_ssaoTex = Coel::findInt(quadShader, "u_ssaoTex");

    Ssao::init(window.size);
    Coel::Renderer::Quad2d quadRenderer;

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        while (window.isOpen) {
            // GBuffer pass
            Coel::bind(Scene::shader);
            Coel::bind(Ssao::gbufferFbo);
            Scene::draw(window);

            // SSAO pass
            Ssao::draw(window.size, quadRenderer, Scene::projMat, Scene::viewMat);

            // Blit pass
            Coel::bind(window.fbo);
            Coel::Renderer::enableDepthTest(false);
            Coel::Renderer::enableCulling(false);
            Coel::bind(quadShader);
            Coel::send(u_quad_posTex, 0);
            Coel::bind(Ssao::gbufferPosTex, 0);
            Coel::send(u_quad_nrmTex, 1);
            Coel::bind(Ssao::gbufferNrmTex, 1);
            Coel::send(u_quad_colTex, 2);
            Coel::bind(Ssao::gbufferColTex, 2);
            Coel::send(u_quad_ssaoTex, 3);
            Coel::bind(Ssao::blurTex, 3);
            quadRenderer.draw();

            if (Scene::is_paused) {
                imgui.begin();
                // ImGui::DragFloat("AO radius", &ao_radius, 0.01f * 360, 0.0f, 360.0f);
                imgui.end(window);
            }

            Coel::update(window);
        }
    }
}
