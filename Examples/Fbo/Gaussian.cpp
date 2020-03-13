#include "GaussianShaders.hpp"
#include <Coel.hpp>

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

out vec4 color;

void main() {
	color = vec4(1, 0, 1, 1);
}
)";

int main() {
    Coel::Window window(1280, 960, "Gaussian Blur Example");

    float vdata[]{
        -0.5, -0.5, // bottom left
        0.0,  0.5,  // top middle
        0.5,  -0.5, // bottom right
    };

    Coel::Vbo vbo(vdata, sizeof(vdata), {{Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);
    Coel::Shader shader(vertSrc, fragSrc);

    Coel::Fbo gaussFboH[2]{
        Coel::Fbo(window.size.x / 2, window.size.y / 2, Coel::Buffer::Color | Coel::Buffer::RenderDepth),
        Coel::Fbo(window.size.x / 4, window.size.y / 4, Coel::Buffer::Color | Coel::Buffer::RenderDepth),
    };
    Coel::Fbo gaussFboV[2]{
        Coel::Fbo(window.size.x / 2, window.size.y / 2, Coel::Buffer::Color | Coel::Buffer::RenderDepth),
        Coel::Fbo(window.size.x / 4, window.size.y / 4, Coel::Buffer::Color | Coel::Buffer::RenderDepth),
    };

    Coel::Shader gaussHorizontalShader(gaussHorizontalVertSrc, gaussFragSrc);
    Coel::Shader gaussVerticalShader(gaussVerticalVertSrc, gaussFragSrc);

    Coel::Renderer::Quad2d quad;

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::enableCulling(true);
    Coel::Renderer::setClearColor(0, 1, 0, 1);

    while (window.isOpen()) {
        gaussFboH[0].bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::clear();

        shader.bind();
        vao.draw(3);

        gaussFboV[0].bind();
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.sendInt("tex", 0);
        gaussHorizontalShader.sendFloat("renderTargetWidth", window.size.x / 2);
        gaussFboH[0].bindColorTexture(0);
        quad.draw();

        gaussFboH[1].bind();
        Coel::Renderer::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.sendInt("tex", 0);
        gaussVerticalShader.sendFloat("renderTargetHeight", window.size.y / 2);
        gaussFboV[0].bindColorTexture(0);
        quad.draw();

        gaussFboV[1].bind();
        Coel::Renderer::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.sendInt("tex", 0);
        gaussHorizontalShader.sendFloat("renderTargetWidth", window.size.x / 4);
        gaussFboH[1].bindColorTexture(0);
        quad.draw();

        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.sendInt("tex", 0);
        gaussVerticalShader.sendFloat("renderTargetHeight", window.size.y / 4);
        gaussFboV[1].bindColorTexture(0);
        quad.draw();

        window.update();
    }
}
