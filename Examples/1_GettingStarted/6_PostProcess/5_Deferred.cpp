#include <Coel.hpp>
#include <iostream>

int main() {
    // Code originates from Model/Simple.cpp
    Coel::Window window({1280, 960}, "Deferred Rendering Example");
    Coel::create(window);

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec4 v_pos;
    out vec3 v_nrm;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        v_pos = u_modlMat * vec4(a_pos, 1);
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * v_pos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec4 v_pos;
    in vec3 v_nrm;
    in vec2 v_tex;
    layout (location = 0) out vec4 g_pos;
    layout (location = 1) out vec3 g_nrm;
    layout (location = 2) out vec4 g_col;
    uniform sampler2D u_tex;
    void main() {
        g_pos = v_pos, g_nrm = v_nrm;
        g_col = texture(u_tex, v_tex);
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
    uniform sampler2D u_posTex;
    uniform sampler2D u_nrmTex;
    uniform sampler2D u_colTex;
    void main() {
        vec2 tex = v_tex * 2;
        if (v_tex.x > 0.5) {
            if (v_tex.y > 0.5)
                color = texture(u_nrmTex, tex);
            else
                color = texture(u_posTex, tex);
        } else {
            vec4 col = texture(u_colTex, tex);
            if (v_tex.y > 0.5)
                color = col;
            else
                color = vec4(col.w);
        }
        color.w = 1;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/stall.obj");
    auto u_tex = shader.findInt("u_tex");
    Coel::Texture texture("Assets/stall.png");

    // We'll create 3 color attachments, the zero-th for position, first for normals, and second for color/specular
    Coel::Fbo gbufferFbo(window.size.x / 2, window.size.y / 2,
                         {Coel::ColorBuffer::RGBA16, Coel::ColorBuffer::RGB16, Coel::ColorBuffer::RGBA8});
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    auto u_posTex = quadShader.findInt("u_posTex");
    auto u_nrmTex = quadShader.findInt("u_nrmTex");
    auto u_colTex = quadShader.findInt("u_colTex");
    Coel::Renderer::Quad2d quadRenderer;

    while (window.isOpen) {
        gbufferFbo.bind();

        shader.bind();
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::setClearColor(0.6, 0.6, 0.8, 1);
        Coel::Renderer::clear();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 20.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -2, -12});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)Coel::getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);

        shader.send(u_tex, 0);
        texture.bind(0);

        model.draw();

        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.send(u_posTex, 0);
        gbufferFbo.bindColorAttachmentTexture(0, 0);
        quadShader.send(u_nrmTex, 1);
        gbufferFbo.bindColorAttachmentTexture(1, 1);
        quadShader.send(u_colTex, 2);
        gbufferFbo.bindColorAttachmentTexture(2, 2);
        quadRenderer.draw();

        Coel::update(window);
    }
}
