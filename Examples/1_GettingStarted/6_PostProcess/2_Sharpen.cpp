#include <Coel.hpp>
#include <iostream>

int main() {
    // Code originates from Model/Simple.cpp
    Coel::Window window("Sharpen Filter Example");
    Coel::create(window);

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
    void main() {
        vec2 off = vec2(1) / textureSize(u_fboTex, 0);
        color = vec4(0);

        color -= texture(u_fboTex, v_tex + vec2(off.x, 0));
        color -= texture(u_fboTex, v_tex + vec2(off.x, off.y));
        color -= texture(u_fboTex, v_tex + vec2(0, off.y));

        color -= texture(u_fboTex, v_tex + vec2(off.x, -off.y));
        color += 9 * texture(u_fboTex, v_tex);
        color -= texture(u_fboTex, v_tex - vec2(off.x, -off.y));
        
        color -= texture(u_fboTex, v_tex - vec2(off.x, 0));
        color -= texture(u_fboTex, v_tex - vec2(off.x, off.y));
        color -= texture(u_fboTex, v_tex - vec2(0, off.y));
        
        color.w = 1;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/dragon.obj");

    Coel::Fbo fbo(window.size.x, window.size.y, {Coel::ColorBuffer::RGBA8});
    Coel::Shader quadShader(quadVertSrc, quadFragSrc);
    auto u_fboTex = quadShader.findInt("u_fboTex");
    Coel::Renderer::Quad2d quadRenderer;

    while (window.isOpen) {
        Coel::Renderer::clear();

        // --------------------------------------------------------------
        // We now must manually bind the desired framebuffer every
        // Time that we want to change the buffer, which is every
        // frame if we want to render offscreen and onscreen in the
        // same frame.
        fbo.bind();

        shader.bind();
        // Enable the desired render traits for the scene to be
        // rendered into the framebuffer
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::setClearColor(0.6, 0.6, 0.8, 1);
        Coel::Renderer::clear();
        // --------------------------------------------------------------

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)Coel::getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);

        model.draw();

        // --------------------------------------------------------------
        // Now we set the default framebuffer
        Coel::Fbo::unbind();
        Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
        Coel::Renderer::enableDepthTest(false);
        Coel::Renderer::enableCulling(false);
        Coel::Renderer::setClearColor(1, 0, 0, 1);
        Coel::Renderer::clearColor();

        quadShader.bind();
        quadShader.send(u_fboTex, 0);
        fbo.bindColorAttachmentTexture(0, 0);
        quadRenderer.draw();
        // --------------------------------------------------------------

        Coel::update(window);
    }
}
