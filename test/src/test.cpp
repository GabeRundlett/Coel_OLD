#include <coel.hpp>
#include <iostream>
#include <math.hpp>

namespace test {
    static constexpr char vert_src[] = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec4 col;
layout(location = 3) in float tid;

out VS_OUT {
	vec2 tex;
	vec4 col;
	float tid;
} v;

void main() {
    v.tex = tex;
    v.col = col / 255;
    v.tid = tid;
    gl_Position = vec4(pos, 0, 1);
}
)";

    static constexpr char geom_src[] = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec2 tex;
	vec4 col;
	float tid;
} g_i[];

out GS_OUT {
	vec2 tex;
	vec4 col;
	float tid;
} g_o;

void main() {
	gl_Position = gl_in[0].gl_Position + vec4(0, 0, 0, 0);
	g_o.tex = g_i[0].tex;
	g_o.col = g_i[0].col;
	g_o.tid = g_i[0].tid;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position + vec4(0, 0, 0, 0);
	g_o.tex = g_i[1].tex;
	g_o.col = g_i[1].col;
	g_o.tid = g_i[1].tid;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position + vec4(0, 0, 0, 0);
	g_o.tex = g_i[2].tex;
	g_o.col = g_i[2].col;
	g_o.tid = g_i[2].tid;
	EmitVertex();
	EndPrimitive();
}
)";

    static constexpr char frag_src[] = R"(
#version 330 core

in GS_OUT {
	vec2 tex;
	vec4 col;
	float tid;
} f;

uniform sampler2D tex0;
uniform sampler2D tex1;
out vec4 color;
void main() {
    switch (int(f.tid + 0.5 * sign(f.tid))) {
    case -1: color = f.col; break;
    case 0: color = texture(tex0, f.tex); break;
    case 1: color = texture(tex1, f.tex); break;
    default: color = vec4(ivec3(gl_FragCoord.x + gl_FragCoord.y) / 500 % 2, 1);
    }
}
)";
} // namespace test

int main() {
    coel::Window window(800, 800, "window");

    coel::Texture ball_t("assets/player.png");
    coel::Texture dirt_t("assets/ground.png");
    coel::Shader shader(coel::ShaderType::Vertex, test::vert_src, coel::ShaderType::Geometry, test::geom_src,
                        coel::ShaderType::Fragment, test::frag_src);
    coel::Material material(&shader, &ball_t, "tex0", &dirt_t, "tex1");

    coel::renderer::batch2d::init();

    math::Vec2 pos = {-0.5, -0.3}, vel = {0.421, 0.1565};

    float prev_time = window.get_time(), prev_tick_time = prev_time;
    unsigned int fpt = 0;
    while (!window.should_close()) {
        const float time = window.get_time(), elapsed = time - prev_time;

        if (time - prev_tick_time > 1.f) {
            std::cout << fpt << '\n';
            prev_tick_time += 1.f;
            fpt = 0;
        }

        coel::renderer::clear(0.1, 0.1, 0.11, 1);

        coel::renderer::batch2d::submit_rect(pos.x, pos.y, 1, 1, 4.f);
        coel::renderer::batch2d::flush();

        pos += vel * elapsed;

        if (pos.x > 0.f) {
            vel.x *= -1;
            pos.x = 0.f;
        } else if (pos.x < -1.f) {
            vel.x *= -1;
            pos.x = -1.f;
        }

        if (pos.y > 0.f) {
            vel.y *= -1;
            pos.y = 0.f;
        } else if (pos.y < -1.f) {
            vel.y *= -1;
            pos.y = -1.f;
        }

        window.update();
        prev_time = time;
        ++fpt;
    }
}
// hi honey i love you !
