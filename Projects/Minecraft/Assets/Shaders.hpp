#pragma once

static constexpr const char *const fragSrc = R"(
#version 450 core

out vec4 fragColor;

void main() {
    fragColor = vec4(1, 0, 1, 1);
}
)";
