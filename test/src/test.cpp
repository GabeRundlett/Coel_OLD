#include <coel.hpp>
#include <iostream>

#include <../dep/debug/src/debug.hpp>

const char *const vert_src = R"(
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
                  *const frag_src = R"(
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

int main() {
    debug::profile::begin("Profile1", "setup.json");
    auto window = coel::create_window(800, 800, "Application Window");

    struct Vertex {
        math::Vec2 pos, tex;
    };

    Vertex vertices[] = {
        {-0.5, -0.5, 0, 0},
        {-0.5, 0.5, 0, 1},
        {0.5, -0.5, 1, 0},
        {0.5, 0.5, 1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}, {coel::Type::f32, 2}});
    auto ibo = coel::create_ibo(indices, sizeof(indices));
    auto vao = coel::create_vao();
    vao->add_vbo(vbo);

    auto shader = coel::create_shader(vert_src, frag_src);

    auto tex0 = coel::create_texture("C:/Users/gabe/OneDrive/Documents/ShareX/Screenshots/2019-10/chrome_6ykxA2PMhv.png");
    auto tex1 = coel::create_texture("C:/Users/gabe/OneDrive/Documents/ShareX/Screenshots/2019-10/chrome_ZNkAmjyU97.png");
    tex0->bind(0);
    tex1->bind(1);

    float r = 0.f;
    debug::profile::end();

    debug::profile::begin("Profile2", "loop.json");
    while (window->is_open()) {
        coel::Renderer::clear();
        vao->draw(ibo);
        window->update();
        coel::Renderer::set_clear_color(r, r, r, 1.f);
        r += 0.01;
        if (r > 1.f) r = 0.f;
    }
    debug::profile::end();
}

// hi honey i love you !
