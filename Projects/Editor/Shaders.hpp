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

float line(in vec2 pos, in vec2 a, in vec2 b, in float radius) {
    vec2 ab = b - a;
    float t = clamp(dot(pos - a, ab) / dot(ab, ab), 0, 1);
    return radius - length(pos - a - t * ab);
}

void main() {
    color = v_col;

	switch(v_material) {
	case 0:
		color = vec4(1, 0, 1, 1);
		break;
	case 1: {
		vec2 size = abs(v_size);
		vec2 uv = v_tex * size;
		float a = clamp(line(uv, vec2(v_border), size - vec2(v_border), v_border), 0, 1);
		color.w *= a;
		break;
	}
	case 2: {
		float l = length(v_tex - 0.5) * 2;
		float s = length(v_size) / 2;
		float a = clamp((1.f - l) * s, 0, 1);
		color.w *= a;
		break;
	}
	}
}
)";
