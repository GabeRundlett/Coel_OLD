#include "../../1_GettingStarted/6_PostProcess/0_Common/Scene.hpp"
#include "../../1_GettingStarted/6_PostProcess/0_Common/Shaders/Deferred.hpp"

int main() {
    Coel::Window window({1280, 960}, "Diffuse Phong Shading Example");
    Coel::create(window);
    Scene::init(Deferred::vertSrc, Deferred::fragSrc, window);

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
    uniform vec3 u_lightPos;
    uniform vec3 u_lightCol;
    uniform float u_lightIntensity;
    void main() {
        vec4 fragPos = texture(u_posTex, v_tex);
        vec4 fragNrm = normalize(texture(u_nrmTex, v_tex));
        vec4 fragCol = texture(u_colTex, v_tex);

        vec3 lightDir = u_lightPos - fragPos.xyz;
        float lightDist = length(lightDir);
        lightDir /= lightDist;
        float lightAttenuation = 1 / pow(lightDist, 2);
        
        float diffuseFactor = max(0.2, dot(lightDir, fragNrm.xyz));
        color = fragCol * diffuseFactor * u_lightCol.xyzz * lightAttenuation * u_lightIntensity;
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
    auto u_lightPos = Coel::findFloat3(quadShader, "u_lightPos");
    auto u_lightCol = Coel::findFloat3(quadShader, "u_lightCol");
    auto u_lightIntensity = Coel::findFloat(quadShader, "u_lightIntensity");
    glm::vec3 lightPos{1.5, 7, 4.5}, lightCol{1, 1, 1};
    Coel::Renderer::Quad2d quadRenderer;
    Coel::Renderer::ImGuiRenderer imgui(window);
    float lightIntensity = 40;

    while (window.isOpen) {
        Coel::bind(gbufferFbo);
        Coel::Renderer::setClearColor(0, 0, 0, 1);
        Scene::draw(window);
        Coel::bind(window.fbo);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clear();

        Coel::bind(quadShader);
        Coel::send(u_lightPos, &lightPos);
        Coel::send(u_lightCol, &lightCol);
        Coel::send(u_lightIntensity, lightIntensity);
        Coel::send(u_posTex, 0);
        Coel::bind(gbufferPosTex, 0);
        Coel::send(u_nrmTex, 1);
        Coel::bind(gbufferNrmTex, 1);
        Coel::send(u_colTex, 2);
        Coel::bind(gbufferColTex, 2);
        quadRenderer.draw();

        imgui.begin();
        ImGui::Begin("Light Settings");
        ImGui::SliderFloat3("Light Position", reinterpret_cast<float *>(&lightPos), -15, 15);
        ImGui::ColorEdit3("Light Color", reinterpret_cast<float *>(&lightCol));
        ImGui::DragFloat("Light lightIntensity", &lightIntensity, 0.01f);
        ImGui::End();
        imgui.end(window);

        Coel::update(window);
    }
}
