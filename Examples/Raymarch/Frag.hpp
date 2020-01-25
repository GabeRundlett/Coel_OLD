#pragma once

static constexpr const char *const sFragSrc = R""(
#version 440
    
in vec2 vTex;
uniform ivec2 size;

out vec4 color;

struct SceneInfo {
    //
};
   
float sphereDE(in vec3 rayPos, in vec3 sPos, in float r) {
    return length(sPos - rayPos) - r;
}
    
SceneInfo sceneInfo(in vec3 rayPos) {
    SceneInfo result;
    return result;
}

void main() {
    color = vec4(vTex, 0, 1);
})"";
