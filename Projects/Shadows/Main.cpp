#include <Coel.hpp>
#include "Shaders.hpp"
#include <iostream>
#include <thread>

Coel::Window window(1280, 960, "Shadow Mapping Example");

struct Vertex {
    Math::Vec3 pos, nrm;
    Math::Vec2 tex;
    Math::Vec4 col;
};

namespace Scene {
    constexpr Math::Vec4 col1{1, 1, 1, 1}, col2{0.4, 0.4, 0.5, 1};
    Vertex vdata[]{
        {{-1, -1, -1}, {0, 0, -1}, {0, 0}, col1}, 
        {{1, -1, -1}, {0, 0, -1}, {1, 0}, col1},
        {{-1, 1, -1}, {0, 0, -1}, {0, 1}, col1},
        {{1, 1, -1}, {0, 0, -1}, {1, 1}, col1},
        {{-1, -1, 1}, {0, 0, 1}, {0, 0}, col1},
        {{-1, 1, 1}, {0, 0, 1}, {0, 1}, col1},
        {{1, -1, 1}, {0, 0, 1}, {1, 0}, col1},
        {{1, 1, 1}, {0, 0, 1}, {1, 1}, col1},
        {{-1, -1, -1}, {0, -1, 0}, {0, 0}, col1},
        {{-1, -1, 1}, {0, -1, 0}, {0, 1}, col1},
        {{1, -1, -1}, {0, -1, 0}, {1, 0}, col1},
        {{1, -1, 1}, {0, -1, 0}, {1, 1}, col1},
        {{-1, 1, -1}, {0, 1, 0}, {0, 0}, col1},
        {{1, 1, -1}, {0, 1, 0}, {1, 0}, col1},
        {{-1, 1, 1}, {0, 1, 0}, {0, 1}, col1},
        {{1, 1, 1}, {0, 1, 0}, {1, 1}, col1},
        {{-1, -1, -1}, {-1, 0, 0}, {0, 0}, col1},
        {{-1, 1, -1}, {-1, 0, 0}, {0, 1}, col1},
        {{-1, -1, 1}, {-1, 0, 0}, {1, 0}, col1},
        {{-1, 1, 1}, {-1, 0, 0}, {1, 1}, col1},
        {{1, -1, -1}, {1, 0, 0}, {0, 0}, col1},
        {{1, -1, 1}, {1, 0, 0}, {1, 0}, col1},
        {{1, 1, -1}, {1, 0, 0}, {0, 1}, col1},
        {{1, 1, 1}, {1, 0, 0}, {1, 1}, col1},

        {{-100, -1, 100}, {0, 1, 0}, {0, 0}, col2},
        {{-100, -1, -100}, {0, 1, 0}, {0, 1}, col2},
        {{100, -1, 100}, {0, 1, 0}, {1, 0}, col2},  
        {{100, -1, -100}, {0, 1, 0}, {1, 1}, col2},
    };
    unsigned int idata[]{0,  1,  2,  1,  3,  2,  4,  5,  6,  5,  7,  6,  8,  9,  10, 9,  11, 10, 12, 13, 14,
                         13, 15, 14, 16, 17, 18, 17, 19, 18, 20, 21, 22, 21, 23, 22, 24, 25, 26, 25, 27, 26};

    Coel::Vao vao;
    Coel::Vbo vbo(vdata, sizeof(vdata), {
        {Coel::Element::F32, 3},
        {Coel::Element::F32, 3},
        {Coel::Element::F32, 2},
        {Coel::Element::F32, 4},
    });
    Coel::Ibo ibo(idata, sizeof(idata));

    Coel::Shader shader(vertSrc, fragSrc);

    void init() {
        vao.add(vbo);
    }

    void render() {
        Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
        Coel::Renderer::clear();

        shader.bind();
        ibo.bind();
        vao.drawIndexed(42);
    }
}

namespace Shadow {
    Coel::Shader shader(vertSrc, fragSrc);

    void render() {
        Coel::Renderer::clearDepth();

        shader.bind();
        Scene::ibo.bind();
        Scene::vao.drawIndexed(42);
    }
}

namespace Quad {
    float vdata[]{
        -1, -1, 0, 0, //
        -0.5, -1, 1, 0, //
        -1, -0.5, 0, 1, //
                 
        -1, -0.5, 0, 1, //
        -0.5, -1, 1, 0, //
        -0.5, -0.5, 1, 1, //
    };

    Coel::Vao vao;
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Vbo vbo(vdata, sizeof(vdata), {
        {Coel::Element::F32, 2},
        {Coel::Element::F32, 2},
    });

    void init() {
        vao.add(vbo);
    }

    void render() {
        Coel::Renderer::setClearColor(0, 0, 0, 1);
        Coel::Renderer::clear();

        shader.bind();
        shader.sendInt("tex", 0);
        vao.draw(6);
    }
}

int main() {
    Scene::init();
    // Quad::init();

    Coel::Fbo shadowFbo(1024, 1024, Coel::Buffer::Depth);

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(true);

    while (window.isOpen()) {
        Math::Vec3 viewPos{0, 0, -5};
        Math::Mat4 projMat = Math::Mat4::perspective(Math::HALF_PI, (float)window.size.x / window.size.y, 0.01, 100.f),
                   viewMat = Math::Mat4::translation(viewPos), modelMat{1};
        Math::Vec2 mouseRot{
            (0.5 - window.mouse.pos.y / window.size.y) * 8,
            (window.mouse.pos.x / window.size.x - 0.5) * 8,
        };
        // viewMat *= Math::Mat4::rotX((0.5 - window.mouse.pos.y / window.size.y) * 8);
        // viewMat *= Math::Mat4::rotY((window.mouse.pos.x / window.size.x - 0.5) * 8);
        viewMat *= Math::Mat4::rotX(-0.5);
        viewMat *= Math::Mat4::rotY(0.5);
        Math::Mat4 shadowProjMat = Math::Mat4::ortho(-2, 2, -2, 2, 0, 10), shadowViewMat{1};
        Math::Vec2 lightRot{-1, window.getTime()};
        shadowViewMat *= Math::Mat4::translation({0, 0, -6});
        shadowViewMat *= Math::Mat4::rotX(lightRot.x);
        shadowViewMat *= Math::Mat4::rotY(lightRot.y);
        Math::Vec3 lightDir = {-std::sin(lightRot.y), -std::sin(lightRot.x), std::cos(lightRot.y)};

        // Render to fbo
        shadowFbo.bind();
        Shadow::shader.bind();

        Shadow::shader.sendMat4("projMat", &shadowProjMat);
        Shadow::shader.sendMat4("viewMat", &shadowViewMat);
        Shadow::shader.sendMat4("modelMat", &modelMat);
        Shadow::render();

        // Render fbo to quad
        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);

        Scene::shader.bind();
        Scene::shader.sendInt("shadowMap", 0);
        shadowFbo.bindDepthTexture(0);
        Scene::shader.sendMat4("lightProjMat", &shadowProjMat);
        Scene::shader.sendMat4("lightViewMat", &shadowViewMat);
        Scene::shader.sendFloat3("lightDir", &lightDir);
        Scene::shader.sendFloat3("viewPos", &viewPos);
        Scene::shader.sendMat4("projMat", &projMat);
        Scene::shader.sendMat4("viewMat", &viewMat);
        Scene::shader.sendMat4("modelMat", &modelMat);
        Scene::render();

        /*
        Quad::shader.bind();
        shadowFbo.bindDepthTexture(0);
        Quad::shader.sendInt("tex", 0);
        Quad::render();

        */

        window.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
