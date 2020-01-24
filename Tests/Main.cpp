#include <Coel.hpp>

unsigned int width = 800, height = 800;
float aspectRatio = static_cast<float>(width) / height;

enum ShaderUniformFlags {
    NONE,
    ASPECT_RATIO
};

unsigned char shaderUniformFlags = NONE;

void windowResize(const Coel::WindowResize &w) {
    width = w.size.x, height = w.size.y;
    aspectRatio = static_cast<float>(width) / height;

    shaderUniformFlags |= ASPECT_RATIO;
}

int main() {
    const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0, 1);
}
)",
                      *const fragSrc = R"(
#version 440

uniform float aspectRatio = 1.f;
uniform float width = 1.f;
uniform float height = 1.f;

#define PI 3.14159265
#define TAU (PI * 2)

out vec4 color;

vec3 hsvToRgb(vec3 col) {
    float h = col.x, s = col.y, v = col.z; 
    float c = s * v;
    float x = c * (1.f - abs(mod(h / TAU * 6, 2) - 1));
    float m = v - c;
    
    vec3 tCol = vec3(0);

    if (h < 0.f) {
        //
    } else if (h < 1 * TAU / 6) {
        tCol = vec3(c, x, 0);
    } else if (h < 2 * TAU / 6) {
        tCol = vec3(x, c, 0);
    } else if (h < 3 * TAU / 6) {
        tCol = vec3(0, c, x);
    } else if (h < 4 * TAU / 6) {
        tCol = vec3(0, x, c);
    } else if (h < 5 * TAU / 6) {
        tCol = vec3(x, 0, c);
    } else if (h < 6 * TAU / 6) {
        tCol = vec3(c, 0, x);
    } else {
        //
    }
    return tCol + m;
}

void main() {
    vec2 c = gl_FragCoord.xy / vec2(width, height) - vec2(0.5);
    c.x *= aspectRatio;

    c *= 5;

    c = vec2(c.x * c.x - c.y * c.y, c.x * c.y * 2);
    
    float len = length(c);
    float cSin = c.y / len;
    float cCos = c.x / len;
    float a = atan(c.y, c.x);
    if (a < 0.f) a = TAU + a;
    
    color = vec4(hsvToRgb(vec3(a, 1, len)), 1);
}
)";

    auto window = Coel::createWindow(width, height, "Application Window");
    window->onWindowResize(windowResize);

    struct Vertex {
        Math::Vec2 pos;
    };
    Vertex vertices[] = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);
    auto shader = Coel::createShader(vertSrc, fragSrc);
    shader->sendFloat("width", width);
    shader->sendFloat("height", height);
    shader->sendFloat("aspectRatio", aspectRatio);

    while (!window->shouldClose()) {
        Coel::Renderer::clear();

        if (shaderUniformFlags & ASPECT_RATIO) {
            shader->sendFloat("width", width);
            shader->sendFloat("height", height);
            shader->sendFloat("aspectRatio", aspectRatio);
            shaderUniformFlags &= ~ASPECT_RATIO;
        }

        vao->draw(ibo);
        window->update();
    }
}

// hi honey i love you !
