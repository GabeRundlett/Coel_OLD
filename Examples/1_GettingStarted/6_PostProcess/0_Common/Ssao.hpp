#pragma once

#include <Coel.hpp>
#include <random>

namespace Ssao {
    const char *const quadVertSrc = R"(
    #version 450
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    })";

    const char *const ssaoFragSrc = R"(
    #version 450
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_posTex;
    uniform sampler2D u_nrmTex;
    uniform sampler2D u_colTex;
    layout(std430, binding = 3) buffer Buf {
        vec4 ssaoKernel[64];
        vec4 ssaoNoise[16];
        mat4 projMat;
        mat4 viewMat;
        vec2 frameDim;
    };
    void main() {
        vec3 pos = (viewMat * vec4(texture(u_posTex, v_tex).xyz, 1)).xyz;
        vec3 nrm = normalize(texture(u_nrmTex, v_tex).xyz);
        vec4 tex_col = texture(u_colTex, v_tex);
        if (tex_col.a == 0)
            discard;
        vec3 col = tex_col.rgb;
        color = vec4(col, 1);
        float radius = 0.5;
        float bias = 0.025;
        ivec2 itex = ivec2(v_tex * frameDim) % 4;
        vec3 randomVec = normalize(ssaoNoise[itex.x + itex.y * 4].xyz);
        vec3 tng = normalize(randomVec - nrm * dot(randomVec, nrm));
        vec3 btn = cross(nrm, tng);
        mat3 TBN = mat3(tng, btn, nrm);
        float occlusion = 0.0;
        for(int i = 0; i < 64; ++i) {
            vec3 samplePos = TBN * ssaoKernel[i].xyz;
            samplePos = pos + samplePos * radius;
            vec4 offset = vec4(samplePos, 1.0);
            offset = projMat * offset;
            offset.xyz /= offset.w;
            offset.xyz = offset.xyz * 0.5 + 0.5;
            if (offset.x < 0 || offset.x > 1 || offset.y < 0 || offset.y > 1)
                continue;
            vec4 samplePosView = viewMat * vec4(texture(u_posTex, offset.xy).xyz, 1);
            float sampleDepth = samplePosView.z;
            // float rangeCheck = smoothstep(0.0, 1.0, radius / abs(pos.z - sampleDepth));
            float rangeCheck = 1.0;
            occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
        }
        occlusion = 1.0 - (occlusion / 64);
        if (pos.z > 1.0 - 0.5)
            occlusion = 1;
        color.r = occlusion;
    })";

    const char *const blurFragSrc = R"(
    #version 440
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_fboTex;
    void main() {
        color = vec4(0);
        const int kernelSize = 2;
        vec2 texelSize = vec2(1.f) / textureSize(u_fboTex, 0);
        for (int y = -kernelSize; y < kernelSize; ++y) {
            for (int x = -kernelSize; x < kernelSize; ++x) {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                color += texture(u_fboTex, v_tex + offset);
            }
        }
        color /= 4 * kernelSize * kernelSize;
        color.w = 1;
    })";

    Coel::Fbo gbufferFbo;
    Coel::Rbo gbufferDepRbo;
    Coel::Texture gbufferPosTex, gbufferNrmTex, gbufferColTex;

    struct SsaoSboData {
        glm::vec4 ssaoNoise[16];
        glm::vec4 ssaoKernel[64];
        glm::mat4 projMat;
        glm::mat4 viewMat;
        glm::vec2 frameDim;
    };
    SsaoSboData ssaoSboData;

    Coel::Fbo ssaoFbo;
    Coel::Rbo ssaoDepRbo;
    Coel::Sbo ssaoSbo;
    Coel::Texture ssaoTex;
    Coel::Shader ssaoShader;
    Coel::Uniform<int> u_ssao_posTex;
    Coel::Uniform<int> u_ssao_nrmTex;
    Coel::Uniform<int> u_ssao_colTex;

    Coel::Fbo blurFbo;
    Coel::Rbo blurDepRbo;
    Coel::Texture blurTex;
    Coel::Shader blurShader;
    Coel::Uniform<int> u_blur_fboTex;

    void init(glm::ivec2 window_size) {
        Coel::create(gbufferPosTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferNrmTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferColTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferDepRbo, window_size);
        Coel::create(gbufferFbo, window_size);
        Coel::attach(gbufferFbo, {gbufferPosTex, gbufferNrmTex, gbufferColTex});
        Coel::attach(gbufferFbo, gbufferDepRbo);

        Coel::create(ssaoTex, window_size, Coel::RED, nullptr);
        Coel::create(ssaoDepRbo, window_size);
        Coel::create(ssaoFbo, window_size);
        Coel::attach(ssaoFbo, {ssaoTex});
        Coel::attach(ssaoFbo, ssaoDepRbo);
        Coel::create(ssaoShader, quadVertSrc, ssaoFragSrc);
        u_ssao_posTex = Coel::findInt(ssaoShader, "u_posTex");
        u_ssao_nrmTex = Coel::findInt(ssaoShader, "u_nrmTex");
        u_ssao_colTex = Coel::findInt(ssaoShader, "u_colTex");

        std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 64; ++i) {
            glm::vec3 sample(
                randomFloats(generator) * 2.0f - 1.0f,
                randomFloats(generator) * 2.0f - 1.0f,
                randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = static_cast<float>(i) / 64.0f;
            scale = std::lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoSboData.ssaoKernel[i] = glm::vec4(sample.x, sample.y, sample.z, 0.0f);
        }
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++) {
            ssaoSboData.ssaoNoise[i] = glm::vec4(
                randomFloats(generator) * 2.0f - 1.0f,
                randomFloats(generator) * 2.0f - 1.0f,
                0.0f,
                0.0f);
        }
        Coel::create(ssaoSbo, nullptr, sizeof(ssaoSboData));

        // Blur (For the SSAO)
        Coel::create(blurTex, window_size, Coel::RGBA, nullptr);
        Coel::create(blurDepRbo, window_size);
        Coel::create(blurFbo, window_size);
        Coel::attach(blurFbo, {blurTex});
        Coel::attach(blurFbo, blurDepRbo);
        Coel::create(blurShader, quadVertSrc, blurFragSrc);
        u_blur_fboTex = Coel::findInt(blurShader, "u_fboTex");
    }

    void draw(glm::ivec2 window_size, Coel::Renderer::Quad2d &quadRenderer, const glm::mat4 &projMat, const glm::mat4 &viewMat) {
        // SSAO pass
        Coel::bind(ssaoFbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::bind(ssaoShader);
        SsaoSboData *mappedSboData = reinterpret_cast<SsaoSboData *>(Coel::open(ssaoSbo));
        ssaoSboData.projMat = projMat;
        ssaoSboData.viewMat = viewMat;
        ssaoSboData.frameDim = glm::vec2(window_size);
        *mappedSboData = ssaoSboData;
        Coel::close(ssaoSbo);
        Coel::bind(ssaoSbo, 3);
        Coel::send(u_ssao_posTex, 0);
        Coel::bind(gbufferPosTex, 0);
        Coel::send(u_ssao_nrmTex, 1);
        Coel::bind(gbufferNrmTex, 1);
        Coel::send(u_ssao_colTex, 2);
        Coel::bind(gbufferColTex, 2);
        quadRenderer.draw();

        // Blur pass
        Coel::bind(blurFbo);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::bind(blurShader);
        Coel::send(u_blur_fboTex, 0);
        Coel::bind(ssaoTex, 0);
        quadRenderer.draw();
    }

} // namespace Ssao
