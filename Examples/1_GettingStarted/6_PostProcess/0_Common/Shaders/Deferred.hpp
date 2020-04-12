#pragma once

namespace Deferred {
    const char *const vertSrc = R"(
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

    const char *const fragSrc = R"(
#version 450 core
in vec4 v_pos;
in vec3 v_nrm;
in vec2 v_tex;
layout (location = 0) out vec4 g_pos;
layout (location = 1) out vec3 g_nrm;
layout (location = 2) out vec4 g_col;
uniform sampler2D u_tex;
void main() {
    g_pos = v_pos, g_nrm = v_nrm;
    g_col = texture(u_tex, v_tex);
})";
}