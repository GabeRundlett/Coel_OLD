#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nrm;
layout (location = 2) in vec2 a_tex;
layout (location = 3) in vec4 a_col;
out vec2 v_tex;

uniform mat4 u_mvp;

void main() {
	v_tex = a_tex;
	gl_Position = vec4(a_pos, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in vec2 v_tex;
out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture(tex, v_tex);
	color.w = 1;
}
)";
