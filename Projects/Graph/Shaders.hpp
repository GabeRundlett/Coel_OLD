#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec2 size;
layout (location = 3) in vec4 col;
layout (location = 4) in vec4 param;

out vec4 vCol;

void main() {
    vCol = col;
	gl_Position = vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in vec4 vCol;
out vec4 color;

void main() {
	color = vCol;
}
)";
