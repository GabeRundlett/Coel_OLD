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

int main() {
    Coel::Window window(800, 600, "Gaussian Blur Example");

    float vdata[]{
        -0.5, -0.5, // bottom left
        0.5,  -0.5, // bottom right
        0.0,  0.5,  // top middle
    };

    Coel::Vbo vbo(vdata, sizeof(vdata), {
        {Coel::Element::F32, 2},
    });

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

    float quadVdata[]{
        -1, -1, 0, 0, //
        1,  -1, 1, 0, //
        -1, 1,  0, 1, //

        -1, 1,  0, 1, //
        1,  -1, 1, 0, //
        1,  1,  1, 1, //
    };

    Coel::Vbo quadVbo(quadVdata, sizeof(quadVdata), {
        {Coel::Element::F32, 2},
        {Coel::Element::F32, 2},
    });

    Coel::Vao quadVao;
    quadVao.add(quadVbo);

    Coel::Renderer::Command::enableBlend(true);
    Coel::Renderer::Command::enableCulling(true);
    Coel::Renderer::Command::setClearColor(0, 1, 0, 1);

    while (window.isOpen()) {
        gaussFboH[0].bind();
        Coel::Renderer::Command::enableDepthTest(true);
        Coel::Renderer::Command::clear();

        shader.bind();
        vao.draw(3);

        gaussFboV[0].bind();
        Coel::Renderer::Command::enableDepthTest(false);
        Coel::Renderer::Command::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.sendInt("tex", 0);
        gaussHorizontalShader.sendFloat("renderTargetWidth", 800 / 2);
        gaussFboH[0].bindColorTexture(0);
        quadVao.draw(6);

        gaussFboH[1].bind();
        Coel::Renderer::Command::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.sendInt("tex", 0);
        gaussVerticalShader.sendFloat("renderTargetHeight", 600 / 2);
        gaussFboV[0].bindColorTexture(0);
        quadVao.draw(6);

        gaussFboV[1].bind();
        Coel::Renderer::Command::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.sendInt("tex", 0);
        gaussHorizontalShader.sendFloat("renderTargetWidth", 800 / 4);
        gaussFboH[1].bindColorTexture(0);
        quadVao.draw(6);

        Coel::Fbo::unbind();
        Coel::Renderer::Command::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::Command::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.sendInt("tex", 0);
        gaussVerticalShader.sendFloat("renderTargetHeight", 600 / 4);
        gaussFboV[1].bindColorTexture(0);
        quadVao.draw(6);

        window.update();
    }
}
