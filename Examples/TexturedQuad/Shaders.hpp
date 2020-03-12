#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 vTex;

void main() {
	vTex = tex;
	gl_Position = vec4(pos * 0.9, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in vec2 vTex;
out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture(tex, vTex);
	color.w = 1;
}
)";
