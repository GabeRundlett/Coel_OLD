#include <Coel.hpp>
#include <iostream>

int main() {
    // Code originates from Model/Simple.cpp
    Coel::Window window(1400, 1000, "Blurring Filter Example");

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec3 v_nrm;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const gaussHorizontalVertSrc = R"(
    #version 440
    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec2 tex;
    out vec2 vTex[11];
    uniform float u_renderTargetWidth;
    void main() {
        gl_Position = vec4(pos, 0, 1);
        float pixelWidth = 1.f / u_renderTargetWidth;
        for (int i = -5; i < 5; ++i) {
            vTex[i + 5] = tex + vec2(pixelWidth * i, 0);
        }
    }
    )";

    const char *const gaussVerticalVertSrc = R"(
    #version 440
    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec2 tex;
    out vec2 vTex[11];
    uniform float u_renderTargetHeight;
    void main() {
        gl_Position = vec4(pos, 0, 1);
        float pixelHeight = 1.f / u_renderTargetHeight;
        for (int i = -5; i < 5; ++i) {
            vTex[i + 5] = tex + vec2(0, pixelHeight * i);
        }
    }
    )";

    const char *const gaussFragSrc = R"(
    #version 440
    in vec2 vTex[11];
    out vec4 color;
    uniform sampler2D u_tex;
    const float gammaPower = 2.2;
    vec4 gammaUndistort(in vec4 val) { return pow(val, vec4(gammaPower)); }
    vec4 gammaRedistort(in vec4 val) { return pow(val, vec4(1.0/gammaPower)); }
    void main() {
        color = vec4(0);
        color += gammaUndistort(texture(u_tex, vTex[0])) * 0.0093;
        color += gammaUndistort(texture(u_tex, vTex[1])) * 0.028002;
        color += gammaUndistort(texture(u_tex, vTex[2])) * 0.065984;
        color += gammaUndistort(texture(u_tex, vTex[3])) * 0.121703;
        color += gammaUndistort(texture(u_tex, vTex[4])) * 0.175713;
        color += gammaUndistort(texture(u_tex, vTex[5])) * 0.198596;
        color += gammaUndistort(texture(u_tex, vTex[6])) * 0.175713;
        color += gammaUndistort(texture(u_tex, vTex[7])) * 0.121703;
        color += gammaUndistort(texture(u_tex, vTex[8])) * 0.065984;
        color += gammaUndistort(texture(u_tex, vTex[9])) * 0.028002;
        color += gammaUndistort(texture(u_tex, vTex[10])) * 0.0093;
        color = gammaRedistort(color);
        color.w = 1;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_nrm;
    out vec4 frag_color;
    void main() {
        frag_color = vec4(v_nrm, 1);
    }
    )";

    const char *const quadVertSrc = R"(
    #version 440
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    const char *const quadFragSrc = R"(
    #version 440
    in vec2 v_tex;
    out vec4 color;
    uniform sampler2D u_fboTex;
    uniform sampler2D u_blurredTex;
    void main() {
        vec4 blurCol = texture(u_blurredTex, v_tex);
        color = blurCol;
        color.w = 1;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/dragon.obj");

    Coel::Fbo gaussFboH[]{
        Coel::Fbo(window.size.x / 2, window.size.y / 2, {Coel::ColorBuffer::RGBA8}),
        Coel::Fbo(window.size.x / 4, window.size.y / 4, {Coel::ColorBuffer::RGBA8}),
    };
    Coel::Fbo gaussFboV[]{
        Coel::Fbo(window.size.x / 2, window.size.y / 2, {Coel::ColorBuffer::RGBA8}),
        Coel::Fbo(window.size.x / 4, window.size.y / 4, {Coel::ColorBuffer::RGBA8}),
    };

    Coel::Shader gaussHorizontalShader(gaussHorizontalVertSrc, gaussFragSrc);
    auto u_gaussHorizontalTex = gaussHorizontalShader.findInt("u_tex");
    auto u_renderTargetWidth = gaussHorizontalShader.findFloat("u_renderTargetWidth");
    Coel::Shader gaussVerticalShader(gaussVerticalVertSrc, gaussFragSrc);
    auto u_gaussVerticalTex = gaussVerticalShader.findInt("u_tex");
    auto u_renderTargetHeight = gaussVerticalShader.findFloat("u_renderTargetHeight");

    Coel::Fbo fbo(window.size.x, window.size.y, {Coel::ColorBuffer::RGBA8});
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    auto u_fboTex = quadShader.findInt("u_fboTex");
    auto u_blurredTex = quadShader.findInt("u_blurredTex");
    auto u_kernelSize = quadShader.findInt("u_kernelSize");
    auto u_mousePosX = quadShader.findFloat("u_mousePosX");
    Coel::Renderer::Quad2d quadRenderer;

    while (window.isOpen()) {
        Coel::Renderer::clear();

        // Render pass
        fbo.bind();

        shader.bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::setClearColor(0.6, 0.6, 0.8, 1);
        Coel::Renderer::clear();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)window.getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);

        model.draw();

        // First horizontal pass
        gaussFboH[0].bind();
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.bind();
        gaussHorizontalShader.send(u_gaussHorizontalTex, 0);
        gaussHorizontalShader.send(u_renderTargetWidth, window.size.x / 2);
        fbo.bindColorAttachmentTexture(0, 0);
        quadRenderer.draw();

        // First vertical pass
        gaussFboV[0].bind();
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.bind();
        gaussVerticalShader.send(u_gaussVerticalTex, 0);
        gaussVerticalShader.send(u_renderTargetHeight, window.size.y / 2);
        gaussFboH[0].bindColorAttachmentTexture(0, 0);
        quadRenderer.draw();

        // Second horizontal pass
        gaussFboH[1].bind();
        Coel::Renderer::clearColor();

        gaussHorizontalShader.bind();
        gaussHorizontalShader.bind();
        gaussHorizontalShader.send(u_gaussHorizontalTex, 0);
        gaussHorizontalShader.send(u_renderTargetWidth, window.size.x / 4);
        gaussFboV[0].bindColorAttachmentTexture(0, 0);
        quadRenderer.draw();

        // Second vertical pass
        gaussFboV[1].bind();
        Coel::Renderer::clearColor();

        gaussVerticalShader.bind();
        gaussVerticalShader.bind();
        gaussVerticalShader.send(u_gaussVerticalTex, 0);
        gaussVerticalShader.send(u_renderTargetHeight, window.size.y / 4);
        gaussFboH[1].bindColorAttachmentTexture(0, 0);
        quadRenderer.draw();

        // Final draw pass
        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.send(u_fboTex, 0);
        fbo.bindColorAttachmentTexture(0, 0);
        quadShader.send(u_blurredTex, 1);
        gaussFboV[1].bindColorAttachmentTexture(0, 1);
        quadShader.send(u_mousePosX, (float)window.mouse.pos.x / window.size.x);

        quadRenderer.draw();

        window.update();
    }
}
