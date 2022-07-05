#pragma once

#include <Coel.hpp>

namespace Deferred {
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
uniform mat4 u_modlNrmMat;
void main() {
    vec4 worldPos = u_modlMat * vec4(a_pos, 1);
    vec4 worldNrm = u_modlNrmMat * vec4(a_nrm, 1);
    vec4 viewPos = u_viewMat * worldPos;
    v_pos = worldPos.xyz;
    v_nrm = normalize(worldNrm.xyz);
    v_tex = a_tex;
    gl_Position = u_projMat * viewPos;
})";

    const char *const fragSrc = R"(
#version 450 core
in vec3 v_pos;
in vec3 v_nrm;
in vec2 v_tex;
layout (location = 0) out vec4 g_pos;
layout (location = 1) out vec4 g_nrm;
layout (location = 2) out vec4 g_col;
uniform sampler2D u_tex;
uniform sampler2D u_nrm_tex;
mat3 get_tbn(vec3 n, vec3 p, vec2 uv) {
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);
    vec3 dp2perp = cross(dp2, n);
    vec3 dp1perp = cross(n, dp1);
    vec3 t = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 b = dp2perp * duv1.y + dp1perp * duv2.y;
    float invmax = inversesqrt(max(dot(t, t), dot(b, b)));
    return mat3(t * invmax, b * invmax, n);
}
void main() {
    g_pos = vec4(v_pos, 1);
    g_nrm = vec4(normalize(get_tbn(normalize(v_nrm), v_pos, v_tex) * normalize(texture(u_nrm_tex, v_tex).xyz * 2.0f - 1.0f)), 0);
    g_col = texture(u_tex, v_tex);
})";

    Coel::Fbo gbufferFbo;
    Coel::Rbo gbufferDepRbo;
    Coel::Texture gbufferPosTex, gbufferNrmTex, gbufferColTex;

    void init(glm::ivec2 window_size) {
        Coel::create(gbufferPosTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferNrmTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferColTex, window_size, Coel::RGBA16F, nullptr);
        Coel::create(gbufferDepRbo, window_size);
        Coel::create(gbufferFbo, window_size);
        Coel::attach(gbufferFbo, {gbufferPosTex, gbufferNrmTex, gbufferColTex});
        Coel::attach(gbufferFbo, gbufferDepRbo);
    }

    void begin() {
        Coel::bind(gbufferFbo);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(0, 0, 0, 0);
        Coel::Renderer::clear();
    }

    void bindUniforms(const Coel::Uniform<int> &u_posTex, int i0, const Coel::Uniform<int> &u_nrmTex, int i1, const Coel::Uniform<int> &u_colTex, int i2) {
        Coel::send(u_posTex, i0);
        Coel::bind(Deferred::gbufferPosTex, i0);
        Coel::send(u_nrmTex, i1);
        Coel::bind(Deferred::gbufferNrmTex, i1);
        Coel::send(u_colTex, i2);
        Coel::bind(Deferred::gbufferColTex, i2);
    }
}