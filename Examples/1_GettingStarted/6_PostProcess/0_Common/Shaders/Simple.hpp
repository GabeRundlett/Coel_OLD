#pragma once

namespace Simple {
    static constexpr const char *const vertSrc = R"(
#version 450 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nrm;
layout (location = 2) in vec2 a_tex;
out vec2 v_tex;
uniform mat4 u_projMat;
uniform mat4 u_viewMat;
uniform mat4 u_modlMat;
void main() {
    v_tex = a_tex;
    vec4 worldPos = u_modlMat * vec4(a_pos, 1);
    gl_Position = u_projMat * u_viewMat * worldPos;
})";

    static constexpr const char *const fragSrc = R"(
#version 450 core
in vec2 v_tex;
out vec4 frag_color;
uniform sampler2D u_tex;
void main() {
    frag_color = texture(u_tex, v_tex);
})";
} // namespace Simple
