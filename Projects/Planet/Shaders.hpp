#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec3 pos;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main() {
	gl_Position = projMat * viewMat * modelMat * vec4(pos, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

out vec4 color;

void main() {
	color = vec4(vec3(1), 1);
}
)";
