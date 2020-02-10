#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

uniform mat4 viewMat;

out vec2 v_tex;

void main() {
	v_tex = tex;
	gl_Position = viewMat * vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in vec2 v_tex;

uniform sampler2D tex;

out vec4 color;

void main() {
	color = vec4(0.3, 0.3, 0.3, 1);
	color = texture(tex, v_tex);
}
)";
