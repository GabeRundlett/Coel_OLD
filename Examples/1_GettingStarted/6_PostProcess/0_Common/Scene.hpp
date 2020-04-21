#pragma once
#include <Coel.hpp>

namespace Scene {
    Coel::Shader shader;
    Coel::Uniform<glm::mat4> u_projMat, u_viewMat, u_modlMat;
    glm::mat4 projMat{1}, viewMat{1}, modlMat{1};
    Coel::Model stallModel;
    Coel::Texture stallTexture;
    Coel::Uniform<int> u_stallTex;

    void init(const char *const vertSrc, const char *const fragSrc) {
        Coel::create(shader, vertSrc, fragSrc);
        u_projMat = Coel::findMat4(shader, "u_projMat");
        u_viewMat = Coel::findMat4(shader, "u_viewMat");
        u_modlMat = Coel::findMat4(shader, "u_modlMat");
        u_stallTex = Coel::findInt(shader, "u_stallTex");

        Coel::create(stallModel, "Assets/Models/stall.obj");
        Coel::create(stallTexture, "Assets/Textures/stall.png");
    }

    void draw(const glm::ivec2 &size, float rotation = -1.f) {
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::clear();
        if (rotation == -1.f) rotation = (float)Coel::getTime();
        projMat = glm::perspective(glm::radians(45.f), (float)size.x / size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -3, -15});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), rotation, {0, 1, 0});
        Coel::send(u_projMat, &projMat);
        Coel::send(u_viewMat, &viewMat);
        Coel::send(u_modlMat, &modlMat);
        Coel::send(u_stallTex, 0);
        Coel::bind(stallTexture, 0);
        Coel::Renderer::draw(stallModel);
    }
} // namespace Scene