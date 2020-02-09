#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec2 size;
layout (location = 3) in vec4 col;
layout (location = 4) in vec4 param;

uniform mat4 viewMat;

out vec2 v_tex;
out vec2 v_size;
out vec4 v_col;
flat out int v_material;
flat out int v_border;

void main() {
	v_tex = tex, v_size = size, v_col = col / 255;
	v_material = int(param.r);
	v_border = int(param.g);
	gl_Position = viewMat * vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in vec2 v_tex;
in vec2 v_size;
in vec4 v_col;
flat in int v_material;
flat in int v_border;

out vec4 color;

void main() {
	color = vec4(0.3, 0.3, 0.3, 1);
}
)";
