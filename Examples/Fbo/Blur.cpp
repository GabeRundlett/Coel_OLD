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

static constexpr const char *const quadVertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 vTex;

void main() {
	vTex = tex;
	gl_Position = vec4(pos, 0, 1);
}
)";

static constexpr const char *const quadFragSrc = R"(
#version 440

in vec2 vTex;
out vec4 color;

uniform sampler2D tex;

vec4 sq(in vec4 col) {
    return col * col;
}

void main() {
	color = texture(tex, vTex);
    return;
    for (float y = -10; y < 10; y += 1) {
        for (float x = -10; x < 10; x += 1) {
            color += sq(texture(tex, vTex + vec2(x / 800 * 2, y / 600 * 2))) / 21 / 21;
        }
    }
    color = sqrt(color);
	color.w = 1;
}
)";

int main() {
    Coel::Window window(800, 600, "Simple Framebuffer Example");

    // Lets render the triangle from the static Vbo example
    float vdata[]{
        -0.5, -0.5, // bottom left
        0.0,  0.5,  // top middle
        0.5,  -0.5, // bottom right
    };

    Coel::Vbo vbo(vdata, sizeof(vdata), {{Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);
    Coel::Shader shader(vertSrc, fragSrc);

    Coel::Fbo fbo(window.size.x / 4, window.size.y / 4, Coel::Buffer::Color | Coel::Buffer::RenderDepth);
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    Coel::Renderer::Quad2d quad;

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::enableCulling(true);

    while (window.isOpen()) {
        fbo.bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::setClearColor(0, 1, 0, 1);
        Coel::Renderer::clear();

        shader.bind();
        vao.draw(3);

        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);

        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::setClearColor(0, 0, 0, 1);
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.sendInt("tex", 0);
        fbo.bindColorTexture(0);
        quad.draw();

        window.update();
    }
}
