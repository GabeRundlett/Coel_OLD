#include <Coel.hpp>
#include <iostream>

int main() {
    const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
uniform float aspect_ratio = 1.f;

out vec2 v_tex;

void main() {
	gl_Position.xy = pos;
	gl_Position.x *= aspect_ratio;

	v_tex = tex;
}
)",
                      *const fragSrc = R"(
#version 440

in vec2 v_tex;
layout(binding = 0) uniform sampler2D tex0;
layout(binding = 1) uniform sampler2D tex1;

out vec4 color;

void main() {
	if (v_tex.x > 0.5) color = texture(tex0, v_tex);
	else color = texture(tex1, v_tex);
}
)";

    auto window = Coel::createWindow(800, 800, "Application Window");

    struct Vertex {
        Math::Vec2 pos, tex;
    };
    Vertex vertices[] = {
        {-0.5, -0.5, 0, 0},
        {-0.5, 0.5, 0, 1},
        {0.5, -0.5, 1, 0},
        {0.5, 0.5, 1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}, {Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);

    auto shader = Coel::createShader(vertSrc, fragSrc);

    auto tex0 = Coel::createTexture("C:/Users/gabe/OneDrive/Documents/ShareX/Screenshots/2019-10/chrome_6ykxA2PMhv.png");
    auto tex1 = Coel::createTexture("C:/Users/gabe/OneDrive/Documents/ShareX/Screenshots/2019-10/chrome_ZNkAmjyU97.png");
    tex0->bind(0);
    tex1->bind(1);

    float r = 0.f;

    while (window->isOpen()) {
        Coel::Renderer::clear();
        vao->draw(ibo);
        window->update();
        Coel::Renderer::setClearColor(r, r, r, 1.f);
        r += 0.01;
        if (r > 1.f) r = 0.f;
    }
}

// hi honey i love you !
