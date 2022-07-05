#include <Coel.hpp>

int main() {
    Coel::Window window(1400, 800, "Marching Cubes - Step 1: Point Generation");
    Coel::Renderer::ImGuiRenderer imgui(window);

    static constexpr unsigned int SIZE_Z = 20, SIZE_Y = 20, SIZE_X = 20, NOISE_SAMPLES = 3;
    static constexpr unsigned int MAX_VERTEX_COUNT = 100000;
    float noiseValues[SIZE_Z][SIZE_Y][SIZE_X];

    const float SCALE = 0.03f, ROUGHNESS = 0.9f;
    for (unsigned int zi = 0; zi < SIZE_Z; ++zi) {
        for (unsigned int yi = 0; yi < SIZE_Y; ++yi) {
            for (unsigned int xi = 0; xi < SIZE_X; ++xi) {
                float val = 0.f, octaveScale = 2.f / SIZE_X, octaveRoughness = 1.f;
                for (unsigned int i = 0; i < NOISE_SAMPLES; ++i) {
                    val += glm::simplex(glm::vec3(xi, yi, zi) * octaveScale) * octaveRoughness;
                    octaveScale *= SCALE, octaveRoughness *= ROUGHNESS;
                }
                noiseValues[zi][yi][xi] = val;
            }
        }
    }

    struct Vertex {
        glm::vec3 pos;
    } * vertices;
    unsigned int vertexCount = 0;

    Coel::Vao vao;
    Coel::Vbo vbo(nullptr, sizeof(Vertex) * MAX_VERTEX_COUNT, {{Coel::Element::F32, 3}});

    vbo.open(&vertices);
    vao.add(vbo);

    auto drawCube = [&vao, &vbo, &vertices, &vertexCount](float x, float y, float z) {
        if (vertexCount > MAX_VERTEX_COUNT) {
            vbo.close();
            vao.draw(vertexCount);
            vbo.open(&vertices);
            vertexCount = 0;
        }
        const float SIZE = 1.f;

        auto setPlaneXY = [&vertices](const float x, const float y, const float z, const float size) {
            *vertices = {{x, y, z}};
            ++vertices;
            *vertices = {{x, y + size, z}};
            ++vertices;
            *vertices = {{x + size, y, z}};
            ++vertices;
            *vertices = {{x, y + size, z}};
            ++vertices;
            *vertices = {{x + size, y, z}};
            ++vertices;
            *vertices = {{x + size, y + size, z}};
            ++vertices;
        };
        auto setPlaneXZ = [&vertices](const float x, const float y, const float z, const float size) {
            *vertices = {{x, y, z}};
            ++vertices;
            *vertices = {{x, y, z + size}};
            ++vertices;
            *vertices = {{x + size, y, z}};
            ++vertices;
            *vertices = {{x, y, z + size}};
            ++vertices;
            *vertices = {{x + size, y, z}};
            ++vertices;
            *vertices = {{x + size, y, z + size}};
            ++vertices;
        };
        auto setPlaneYZ = [&vertices](const float x, const float y, const float z, const float size) {
            *vertices = {{x, y, z}};
            ++vertices;
            *vertices = {{x, y, z + size}};
            ++vertices;
            *vertices = {{x, y + size, z}};
            ++vertices;
            *vertices = {{x, y, z + size}};
            ++vertices;
            *vertices = {{x, y + size, z}};
            ++vertices;
            *vertices = {{x, y + size, z + size}};
            ++vertices;
        };

        setPlaneXY(x, y, z, SIZE);
        setPlaneXY(x, y, z + SIZE, SIZE);

        setPlaneXZ(x, y, z, SIZE);
        setPlaneXZ(x, y + SIZE, z, SIZE);

        setPlaneYZ(x, y, z, SIZE);
        setPlaneYZ(x, y, z + SIZE, SIZE);

        vertexCount += 6 * 6;
    };

    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos * 1.f / 20, 1);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    out vec4 frag_color;
    void main() {
        frag_color = vec4(1, 1, 1, 0.1);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat{1}, viewMat{1}, modlMat{1};

    float cutoff = 0;

    while (window.isOpen()) {
        Coel::Renderer::clear();

        shader.bind();

        projMat = glm::perspective(glm::radians(35.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -0.8, -3});
        viewMat = glm::rotate(viewMat, glm::radians(-20.f), {1, 0, 0});
        viewMat = glm::rotate(viewMat, glm::radians(-35.f), {0, 1, 0});

        modlMat = glm::rotate(glm::identity<glm::mat4>(), glm::radians((float)window.getTime()), {0, 1, 0});
        modlMat = glm::rotate(modlMat, glm::radians(0.f), {1, 0, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);

        for (unsigned int zi = 0; zi < SIZE_Z; ++zi) {
            for (unsigned int yi = 0; yi < SIZE_Y; ++yi) {
                for (unsigned int xi = 0; xi < SIZE_X; ++xi) {
                    if (noiseValues[zi][yi][xi] > cutoff)
                        drawCube(xi, yi, zi);
                }
            }
        }

        vbo.close();
        vao.draw(vertexCount);
        vbo.open(&vertices);
        vertexCount = 0;

        imgui.begin();
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Cutoff", &cutoff, -1, 1);
        ImGui::End();
        imgui.end(window);

        window.update();
    }
}
