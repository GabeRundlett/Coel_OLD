#include <Coel.hpp>
#include "Assets/Shaders.hpp"

Coel::Window window(1280, 960, "Minecraft Clone");

class ImageRenderer {
    static inline Coel::Vao s_vao;
    static inline float s_vboData[12]{-1, -1, -1, 1, 1, -1, -1, 1, 1, -1, 1, 1};
    static inline Coel::Vbo s_vbo{s_vboData, sizeof(s_vboData), {{Coel::Element::F32, 2}}};
    static inline unsigned int s_flags = 0;

    static inline constexpr const char *const s_vertSrc = R"(#version 450 core
        layout(location=0)in vec2 aPos;layout(location=1)in vec2 aTex;out vec2 vTex;
        void main(){vTex=aTex;gl_Position=vec4(aPos,0,1);})";
    static inline constexpr const char *const s_quadFragSrc = R"(#version 450 core
        in vec2 vTex;out vec4 fragColor;uniform sampler2D image;
        void main(){fragColor=vec4(image,vTex);})";

    static inline Coel::Shader s_quadShader{s_vertSrc, s_quadFragSrc};

    unsigned int m_width, m_height;
    Coel::Shader m_shader;
    Coel::Texture m_texture;
    Coel::Fbo m_fbo;

  public:
    ImageRenderer(const unsigned int width, const unsigned int height, const char *const fragSrc)
        : m_width(width), m_height(height), m_shader(s_vertSrc, fragSrc), m_texture(width, height, nullptr),
          m_fbo(width, height, Coel::Buffer::Color | Coel::Buffer::RenderDepth) {
        if (s_flags == 0) s_vao.add(s_vbo);
    }

    void render() {
        m_fbo.bind();
        Coel::Renderer::Command::resizeViewport(0, 0, m_width, m_height);
        m_shader.bind();
        m_shader.sendInt("image", 0);
        m_texture.bind(0);
        s_vao.draw(6);
        m_fbo.unbind();
    }

    void draw() {
        s_quadShader.sendInt("image", 0);
        m_fbo.bindColorTexture(0);
    }

    const auto &getTexture() const { return m_texture; }
};

int main() {
    ImageRenderer image(512, 512, fragSrc);

    while (window.isOpen()) {
        image.render();

        window.update();
    }
}
