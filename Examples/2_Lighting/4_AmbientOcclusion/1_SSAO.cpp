#include <Coel.hpp>
#include <iostream>
#include <random>

int main() {
    // Code originates from PostProcessing/Deferred.cpp as well as Phong/Diffuse
    Coel::Window window(800, 600, "Screen Space Ambient Occlusion Example");

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec3 v_pos;
    out vec3 v_nrm;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        v_pos = worldPos.xyz;
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_pos;
    in vec3 v_nrm;
    in vec2 v_tex;
    layout (location = 0) out vec3 g_pos;
    layout (location = 1) out vec3 g_nrm;
    layout (location = 2) out vec4 g_col;
    void main() {
        g_pos = v_pos, g_nrm = v_nrm;
        g_col = vec4(vec3(0.95), 0);
    }
    )";

    const char *const quadVertSrc = R"(
    #version 440
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    const char *const quadFragSrc = R"(
    #version 440
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_posTex;
    uniform sampler2D u_nrmTex;
    uniform sampler2D u_colTex;
    uniform sampler2D u_noise;

    #define SAMPLE_COUNT 64
    uniform vec3 u_ssaoSamples[SAMPLE_COUNT];
    uniform mat4 u_VP;

    void main() {
        vec3 u_lightPos = vec3(5, 10, 3);
    
        vec3 pos = texture(u_posTex, v_tex).xyz;
        vec3 nrm = texture(u_nrmTex, v_tex).xyz;
        vec4 col = texture(u_colTex, v_tex);

        // START SSAO
        vec2 noiseScale = vec2(800, 600) / 4 / 1;
        vec3 randomVec = normalize(texture(u_noise, v_tex * noiseScale).xyz);
        vec3 tangent = normalize(randomVec - nrm * dot(randomVec, nrm));
        vec3 bitangent = cross(nrm, tangent);
        mat3 TBN = mat3(tangent, bitangent, nrm);

        const float radius = 0.01f, bias = 0.1f;
        float occlusion = 0.f;
        vec3 samplePos, occluderPos;
        for (int i = 0; i < SAMPLE_COUNT; ++i) {
            samplePos = TBN * u_ssaoSamples[i];
            samplePos = pos + samplePos * radius;
            vec4 offset = u_VP * vec4(samplePos, 1);
            offset.xyz /= offset.w;
            offset.xyz = offset.xyz * 0.5 + 0.5;
            occluderPos = texture(u_posTex, offset.xy).rgb;
            if (occluderPos.z >= samplePos.z + bias)
                occlusion += 1.f / SAMPLE_COUNT;
        }
        // END SSAO

        vec3 toL = u_lightPos - pos;
        float distToL = length(toL);
        toL /= distToL;

        float diff = max(dot(toL, nrm), 0);

        // color = vec4(col.rgb * diff, 1);
        color = vec4(vec3(occluderPos), 1);
        color.w = 1;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/dragon.obj");

    // We'll create 3 color attachments, the zero-th for position, first for normals, and second for color/specular
    Coel::Fbo gbufferFbo(window.size.x, window.size.y,
                         {Coel::ColorBuffer::RGB16, Coel::ColorBuffer::RGB16, Coel::ColorBuffer::RGBA8});
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    auto u_posTex = quadShader.findInt("u_posTex");
    auto u_nrmTex = quadShader.findInt("u_nrmTex");
    auto u_colTex = quadShader.findInt("u_colTex");
    auto u_noise = quadShader.findInt("u_noise");
    auto u_VP = quadShader.findMat4("u_VP");
    Coel::Renderer::Quad2d quadRenderer;

    glm::i8vec3 ssaoNoiseTextureData[16];

    {
        constexpr auto SAMPLE_COUNT = 64;
        auto u_ssaoSamples = quadShader.findFloat3("u_ssaoSamples");
        std::uniform_real_distribution<float> randomFloatDistrib(0, 1);
        std::default_random_engine gen;
        glm::vec3 samples[SAMPLE_COUNT];
        u_ssaoSamples.count = SAMPLE_COUNT;
        for (unsigned int i = 0; i < SAMPLE_COUNT; ++i) {
            samples[i] = {
                randomFloatDistrib(gen) * 2 - 1,
                randomFloatDistrib(gen) * 2 - 1,
                randomFloatDistrib(gen),
            };
            samples[i] = glm::normalize(samples[i]);
            samples[i] *= randomFloatDistrib(gen);
            float scale = 1.f / SAMPLE_COUNT;
            samples[i] *= scale * scale + 0.1f;
        }

        quadShader.send(u_ssaoSamples, samples);
        for (auto &nrm : ssaoNoiseTextureData) {
            nrm = {
                char(randomFloatDistrib(gen) * 256 - 128),
                char(randomFloatDistrib(gen) * 256 - 128),
                0,
            };
        }
    }
    Coel::Texture ssaoNoiseTexture(4, 4, (const unsigned char *)ssaoNoiseTextureData);

    while (window.isOpen()) {
        Coel::Renderer::clear();

        gbufferFbo.bind();

        shader.bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::setClearColor(0.6, 0.6, 0.8, 1);
        Coel::Renderer::clear();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)window.getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);

        model.draw();

        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.send(u_posTex, 0);
        gbufferFbo.bindColorAttachmentTexture(0, 0);
        quadShader.send(u_nrmTex, 1);
        gbufferFbo.bindColorAttachmentTexture(1, 1);
        quadShader.send(u_colTex, 2);
        gbufferFbo.bindColorAttachmentTexture(2, 2);
        quadShader.send(u_noise, 3);
        ssaoNoiseTexture.bind(3);
        glm::mat4 vp = projMat * viewMat;
        quadShader.send(u_VP, &vp);
        quadRenderer.draw();

        window.update();
    }
}
