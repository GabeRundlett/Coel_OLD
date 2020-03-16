#pragma once

static constexpr const char *const vertSrc = R"(
#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aCol;
out vec4 vCol;
void main() {
	vCol = aCol;
	gl_Position = vec4(aPos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 450 core
in vec4 vCol;
out vec4 fragColor;
void main() {
	fragColor = vCol;
}
)";
