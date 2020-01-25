#pragma once

const char *const sVertSrc = R""(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 vTex;

void main() {
    vTex = tex;
	gl_Position = vec4(pos, 0, 1);
})"";
