#include "Shaders.hpp"
#include "imGuIZMOquat.h"
#include <Coel.hpp>

int main() {
    Coel::Window window(1400, 900, "Minecraft Clone");
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Renderer::ImGuiRenderer imgui(window);

    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    Coel::Vbo vbo(vertices, sizeof(vertices), {{Coel::Element::F32, 3}, {Coel::Element::F32, 3}, {Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    glm::mat4 projMat, viewMat, modlMat;
    glm::vec3 lightDir{0, 2, 1}, lightCol{1, 1, 1}, objectCol{1, 1, 1}, ambientCol{0.2, 0.2, 0.2}, camPos{0, 0, -8};
    float ambientStrength = 0.1f;

    Coel::Renderer::enableDepthTest(true);

    while (window.isOpen()) {
        Coel::Renderer::setClearColor(ambientCol.r, ambientCol.g, ambientCol.b, 1);
        Coel::Renderer::clear();

        shader.bind();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.1f, 100.f);
        shader.sendMat4("projMat", &projMat);
        viewMat = glm::translate(glm::mat4{1}, camPos);
        shader.sendMat4("viewMat", &viewMat);
        modlMat = glm::rotate(glm::mat4{1}, (float)window.getTime(), {0, 1, 1});
        shader.sendMat4("modlMat", &modlMat);

        shader.sendFloat3("camPos", &camPos);
        shader.sendFloat3("objectCol", &objectCol);
        shader.sendFloat3("lightDir", &lightDir);
        shader.sendFloat3("lightCol", &lightCol);
        shader.sendFloat3("ambientCol", &ambientCol);
        shader.sendFloat("ambientStrength", ambientStrength);

        vao.draw(36);

        glm::vec3 blackCol{0.5, 0.5, 0.5};
        shader.sendFloat3("objectCol", &blackCol);
        modlMat = glm::rotate(glm::translate(glm::mat4{1}, {-3, 0, 0}), (float)window.getTime(), {0, 1, 1});
        shader.sendMat4("modlMat", &modlMat);

        vao.draw(36);

        modlMat = glm::rotate(glm::translate(glm::mat4{1}, {3, 0, 0}), (float)window.getTime(), {0, 1, 1});
        shader.sendMat4("modlMat", &modlMat);

        vao.draw(36);

        imgui.begin();
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Object color", (float *)&objectCol);
        ImGui::ColorEdit3("Light color", (float *)&lightCol);

        vec3 dir = vec3(-lightDir.x, -lightDir.y, -lightDir.z);
        if (ImGui::gizmo3D("##Direction", dir)) lightDir = {-dir.x, -dir.y, -dir.z};

        ImGui::ColorEdit3("Ambient color", (float *)&ambientCol);
        ImGui::SliderFloat("Ambient strength", (float *)&ambientStrength, 0, 1);
        ImGui::End();
        imgui.end(window);

        lightDir = glm::normalize(lightDir);

        window.update();
    }
}
