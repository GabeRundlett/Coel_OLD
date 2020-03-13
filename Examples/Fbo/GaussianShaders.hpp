#pragma once

static constexpr const char *const gaussHorizontalVertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 vTex[11];

uniform float renderTargetWidth;

void main() {
	gl_Position = vec4(pos, 0, 1);
    float pixelWidth = 1.f / renderTargetWidth;
    for (int i = -5; i < 5; ++i) {
        vTex[i + 5] = tex + vec2(pixelWidth * i, 0);
    }
}
)";

static constexpr const char *const gaussVerticalVertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 vTex[11];

uniform float renderTargetHeight;

void main() {
	gl_Position = vec4(pos, 0, 1);
    float pixelHeight = 1.f / renderTargetHeight;
    for (int i = -5; i < 5; ++i) {
        vTex[i + 5] = tex + vec2(0, pixelHeight * i);
    }
}
)";

static constexpr const char *const gaussFragSrc = R"(
#version 440

in vec2 vTex[11];
out vec4 color;

uniform sampler2D tex;

const float gammaPower = 2.2;
vec4 gammaUndistort(in vec4 val) { return pow(val, vec4(gammaPower)); }
vec4 gammaRedistort(in vec4 val) { return pow(val, vec4(1.0/gammaPower)); }

void main() {
	color = vec4(0);
    color += gammaUndistort(texture(tex, vTex[0])) * 0.0093;
    color += gammaUndistort(texture(tex, vTex[1])) * 0.028002;
    color += gammaUndistort(texture(tex, vTex[2])) * 0.065984;
    color += gammaUndistort(texture(tex, vTex[3])) * 0.121703;
    color += gammaUndistort(texture(tex, vTex[4])) * 0.175713;
    color += gammaUndistort(texture(tex, vTex[5])) * 0.198596;
    color += gammaUndistort(texture(tex, vTex[6])) * 0.175713;
    color += gammaUndistort(texture(tex, vTex[7])) * 0.121703;
    color += gammaUndistort(texture(tex, vTex[8])) * 0.065984;
    color += gammaUndistort(texture(tex, vTex[9])) * 0.028002;
    color += gammaUndistort(texture(tex, vTex[10])) * 0.0093;
    color = pow(color, vec4(1.0 / 2.2));
	color.w = 1;
}
)";
