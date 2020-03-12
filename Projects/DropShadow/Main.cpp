#include <Coel.hpp>

struct Vertex {
    Math::Vec3 pos;
};
static constexpr unsigned int MAX_VCOUNT = 1000, MAX_ICOUNT = 1000, VBO_SIZE = sizeof(Vertex) * MAX_VCOUNT,
                              IBO_SIZE = sizeof(unsigned short) * MAX_ICOUNT;

static constexpr const char *const pass1VertSrc = R"(
#version 440

layout (location = 0) in vec3 pos;

void main() {
	gl_Position = vec4(pos, 1);
}
)";

static constexpr const char *const pass1FragSrc = R"(
#version 440

out vec4 color;

void main() {
	color = vec4(0.3, 0.5, 0.6, 1);
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

uniform sampler2D colorTex;
uniform sampler2D depthTex;

out vec4 color;

void main() {
	color = texture(colorTex, vTex);
	color += texture(depthTex, vTex) / 3;
    // color += vec4(vTex, 0, 0);
}
)";

int main() {
    Coel::Window window(1280, 960, "Title");

    // First pass setup
    Coel::Vao pass1Vao;
    Coel::Vbo pass1Vbo(nullptr, VBO_SIZE, {
        {Coel::Element::F32, 3},
    });
    pass1Vao.add(pass1Vbo);
    Coel::Ibo pass1Ibo(nullptr, IBO_SIZE);
    unsigned int pass1IndexCount = 0;

    Coel::Fbo pass1Fbo(window.size.x, window.size.y, Coel::Buffer::Color | Coel::Buffer::Depth);
    Coel::Shader pass1Shader(pass1VertSrc, pass1FragSrc);

    // Quad pass setup
    Coel::Vao quadVao;
    float quadVdata[]{
        -1, -1, 0, 0, -1, 1, 0, 1, 1, -1, 1, 0,
        -1,  1, 0, 1,  1, 1, 1, 1, 1, -1, 1, 0,
    };
    Coel::Vbo quadVbo(quadVdata, sizeof(quadVdata), {
        {Coel::Element::F32, 2},
        {Coel::Element::F32, 2},
    });
    quadVao.add(quadVbo);

    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    Coel::Renderer::Command::enableCulling(true);
    Coel::Renderer::Command::enableBlend(true);
    Coel::Renderer::Command::setClearColor(0.1, 0.1, 0.1, 1);

    while (window.isOpen()) {
        pass1Fbo.bind();
        Coel::Renderer::Command::enableDepthTest(true);
        Coel::Renderer::Command::clear();

        {
            Vertex *vertices;
            pass1Vbo.open(&vertices);
            unsigned short *indices;
            pass1Ibo.open(&indices);

            *vertices = {{0, 0, 0}};
            ++vertices;
            *vertices = {{0, 1, 0}};
            ++vertices;
            *vertices = {{1, 0, 0}};
            ++vertices;
            *vertices = {{1, 1, 0}};
            ++vertices;

            *indices = 0;
            ++indices;
            *indices = 1;
            ++indices;
            *indices = 2;
            ++indices;
            *indices = 1;
            ++indices;
            *indices = 3;
            ++indices;
            *indices = 2;
            ++indices;

            pass1IndexCount += 6;

            pass1Ibo.close();
            pass1Vbo.close();

            pass1Shader.bind();
            pass1Ibo.bind();
            pass1Vao.drawIndexed(pass1IndexCount);
            pass1IndexCount = 0;
        }

        Coel::Fbo::unbind();
        Coel::Renderer::Command::resizeViewport(0, 0, window.size.x, window.size.y);

        Coel::Renderer::Command::enableDepthTest(false);
        Coel::Renderer::Command::clearColor();

        {
            quadShader.bind();

            quadShader.sendInt("colorTex", 0);
            pass1Fbo.bindColorTexture(0);

            quadShader.sendInt("depthTex", 1);
            pass1Fbo.bindDepthTexture(1);

            quadVao.draw(6);
        }

        window.update();
    }
}
