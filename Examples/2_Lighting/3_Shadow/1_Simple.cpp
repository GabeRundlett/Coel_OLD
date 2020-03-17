#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window(512, 512, "Simple Model Loading Example");

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec3 v_pos;
    out vec3 v_lightSpacePos;
    out vec3 v_nrm;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_lightProjMat;
    uniform mat4 u_lightViewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        vec4 worldNrm = u_modlMat * vec4(a_nrm, 1);
        v_pos = worldPos.xyz;
        vec4 lightSpacePos = u_lightProjMat * u_lightViewMat * worldPos;
        v_lightSpacePos = lightSpacePos.xyz / lightSpacePos.w / 2 + 0.5;
        v_nrm = worldNrm.xyz, v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_pos;
    in vec3 v_lightSpacePos;
    in vec3 v_nrm;
    in vec2 v_tex;
    out vec4 frag_color;
    uniform sampler2D u_tex;
    uniform sampler2D u_shadowMap;
    uniform vec3 u_lightDir;

    float shadowCalc(in float dotLightNrm) {
        vec3 pos = v_lightSpacePos;
        if (pos.z > 1) pos.z = 1;
        
        float factor = 0.f, bias = max(0.05 * (dotLightNrm), 0.005);
        // float difference = texture(u_shadowMap, pos.xy).r + bias - pos.z;
        vec2 texelSize = 1.f / textureSize(u_shadowMap, 0);

        for (int y = -5; y <= 5; ++y) {
            for (int x = -5; x <= 5; ++x) {
                float depth = texture(u_shadowMap, pos.xy + vec2(float(x), float(y)) * texelSize).r;
                if (depth + bias < pos.z)
                    factor += 0.1111111f;
            }
        }
        return 1 - factor;
    }

    void main() {
        // vec3 v_col = texture(u_tex, v_tex).rgb;
        vec3 v_col = vec3(0.4);
        vec3 lightCol = vec3(1);

        float dotLightNrm = dot(u_lightDir, v_nrm);
        float diff = max(dotLightNrm, 0);
        diff = max(diff, 0.04);
        diff *= shadowCalc(dotLightNrm);

        vec3 color = lightCol * v_col * diff;

        // if (gl_FragCoord.x > 256)
        color = pow(max(color, 0), vec3(1.f / 2.2));
        frag_color = vec4(color, 1);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_lightProjMat = shader.findMat4("u_lightProjMat");
    auto u_lightViewMat = shader.findMat4("u_lightViewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/dragon.obj");
    auto u_tex = shader.findInt("u_tex");
    Coel::Texture texture("Assets/stall.png");

    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(true);
    Coel::Renderer::setClearColor(0, 0, 0, 1);

    Coel::Fbo shadowMapFbo(512, 512, {}, Coel::DepthBuffer::Float32);
    auto u_shadowMap = shader.findInt("u_shadowMap");
    auto u_lightDir = shader.findFloat3("u_lightDir");

    const char *const quadVertSrc = R"(
    #version 440
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1) * vec4(0.3, 0.3, 1, 1) + vec4(0.7, 0.7, 0, 0);
    }
    )";

    const char *const quadFragSrc = R"(
    #version 440
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_fboTex;
    void main() {
        color = texture(u_fboTex, v_tex);
        color.w = 1;
    }
    )";
    Coel::Renderer::Quad2d quadRenderer;
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    auto u_fboTex = quadShader.findInt("u_fboTex");

    while (window.isOpen()) {
        shadowMapFbo.bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::clearDepth();

        glm::mat4 shadowMapProjMat = glm::ortho(-7.f, 7.f, -10.f, 10.f, -12.f, 12.f);
        shadowMapProjMat = glm::rotate(shadowMapProjMat, glm::radians(45.f), {1, 0, 0});
        shadowMapProjMat = glm::rotate(shadowMapProjMat, glm::radians(45.f), {0, 1, 0});
        glm::mat4 shadowMapViewMat = glm::identity<glm::mat4>();
        glm::vec3 lightDir = glm::normalize(glm::vec3(shadowMapProjMat * glm::vec4{0, 0, -1, 1}));

        shader.bind();
        shader.send(u_projMat, &shadowMapProjMat);
        shader.send(u_viewMat, &shadowMapViewMat);
        shader.send(u_modlMat, &modlMat);
        model.draw();

        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::clear();

        shader.bind();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), 0 * (float)window.getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_lightProjMat, &shadowMapProjMat);
        shader.send(u_lightViewMat, &shadowMapViewMat);
        shader.send(u_modlMat, &modlMat);

        shader.send(u_modlMat, &modlMat);
        texture.bind(0);
        shader.send(u_tex, 0);
        shadowMapFbo.bindDepthTexture(1);
        shader.send(u_shadowMap, 1);
        shader.send(u_lightDir, &lightDir);

        model.draw();

        /*
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);

        quadShader.bind();
        quadShader.send(u_fboTex, 0);
        shadowMapFbo.bindDepthTexture(0);
        quadRenderer.draw();
        */

        window.update();
    }
}
