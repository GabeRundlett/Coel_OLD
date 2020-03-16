#include <Coel.hpp>
#include "Shaders.hpp"

constexpr unsigned int COUNT_X = 800 / 4, COUNT_Y = 600 / 4, QUAD_COUNT = COUNT_X * COUNT_Y, VERTEX_COUNT = QUAD_COUNT * 4,
                       INDEX_COUNT = QUAD_COUNT * 6;
struct Vertex {
    float x, y;
    glm::vec4 col;
};
struct QuadVertices {
    Vertex a, b, c, d;
};
struct QuadIndices {
    unsigned int a1, a2, a3, b1, b2, b3;
};

struct HeightMapOptions {
    float octaves = 4, roughness = 0.6, smoothness = 40, seed = 0;
    inline constexpr bool operator==(const HeightMapOptions &o) {
        return o.octaves == octaves && roughness == o.roughness && smoothness == o.smoothness && seed == o.seed;
    }
} heightMapOptions, prevOptions;
float heightMap[COUNT_Y][COUNT_X];

float waterHeight = 0.4, sandHeight = 0.44, snowHeight = 0.67;
glm::vec4 waterColor{0.5, 0.6, 0.9, 1}, sandColor{0.9, 0.8, 0.5, 1}, grassColor{0.4, 0.7, 0.2, 1}, snowColor{0.9, 0.9, 0.95, 1};

void regenHeightMap() {
    for (int yi = 0; yi < COUNT_Y; ++yi) {
        for (int xi = 0; xi < COUNT_X; ++xi) {
            float value = 0;
            float accumulatedAmps = 0;

            for (int i = 0; i < heightMapOptions.octaves; i++) {
                float frequency = glm::pow(2.0f, i);
                float amplitude = glm::pow(heightMapOptions.roughness, i);

                float x = xi * frequency / heightMapOptions.smoothness;
                float y = yi * frequency / heightMapOptions.smoothness;

                float noise =
                    glm::simplex(glm::vec3{heightMapOptions.seed + x, heightMapOptions.seed + y, heightMapOptions.seed});
                noise = (noise + 1.0f) / 2.0f;
                value += noise * amplitude;
                accumulatedAmps += amplitude;
            }

            heightMap[yi][xi] = value / accumulatedAmps;
        }
    }
}

int main() {
    Coel::Window window(800, 600, "Tile Game");
    Coel::Shader shader(vertSrc, fragSrc);

    Coel::Renderer::ImGuiRenderer imgui(window);

    Coel::Vbo vbo(nullptr, VERTEX_COUNT * sizeof(Vertex), {{Coel::Element::F32, 2}, {Coel::Element::F32, 4}});
    QuadVertices *quadVertices = nullptr;

    Coel::Ibo ibo(nullptr, INDEX_COUNT * sizeof(unsigned int));
    QuadIndices *quadIndices = nullptr;

    Coel::Vao vao;
    vao.add(vbo);

    regenHeightMap();

    while (window.isOpen()) {
        Coel::Renderer::clear();

        vbo.open(&quadVertices);
        ibo.open(&quadIndices);
        for (int yi = 0; yi < COUNT_Y; ++yi) {
            for (int xi = 0; xi < COUNT_X; ++xi) {
                const auto index = xi + yi * COUNT_X;

                const auto h = heightMap[yi][xi];
                glm::vec4 color = grassColor;
                if (h < waterHeight)
                    color = waterColor;
                else if (h < sandHeight)
                    color = sandColor;
                else if (h > snowHeight)
                    color = snowColor;

                quadVertices[index] = {
                    {-1.f / COUNT_X + (2.f * xi - COUNT_X + 1.f) / COUNT_X,
                     -1.f / COUNT_Y + (2.f * yi - COUNT_Y + 1.f) / COUNT_Y, color},
                    {1.f / COUNT_X + (2.f * xi - COUNT_X + 1.f) / COUNT_X,
                     -1.f / COUNT_Y + (2.f * yi - COUNT_Y + 1.f) / COUNT_Y, color},
                    {-1.f / COUNT_X + (2.f * xi - COUNT_X + 1.f) / COUNT_X,
                     1.f / COUNT_Y + (2.f * yi - COUNT_Y + 1.f) / COUNT_Y, color},
                    {1.f / COUNT_X + (2.f * xi - COUNT_X + 1.f) / COUNT_X, 1.f / COUNT_Y + (2.f * yi - COUNT_Y + 1.f) / COUNT_Y,
                     color},
                };
                const unsigned int offset = index * 4;
                quadIndices[index] = {
                    offset + 0, offset + 1, offset + 2, offset + 1, offset + 3, offset + 2,
                };
            }
        }
        ibo.close();
        vbo.close();
        vao.drawIndexed(INDEX_COUNT);

        imgui.begin();
        ImGui::Begin("Settings");
        ImGui::Text("Noise options");
        ImGui::SliderFloat("octaves", &heightMapOptions.octaves, 1, 5);
        ImGui::SliderFloat("roughness", &heightMapOptions.roughness, 0, 0.99);
        ImGui::SliderFloat("smoothness", &heightMapOptions.smoothness, 0.1, 100);
        ImGui::SliderFloat("seed", &heightMapOptions.seed, -10, 10);
        ImGui::Separator();
        ImGui::Text("Blockgen settings");
        ImGui::SliderFloat("water", &waterHeight, 0, 0.99);
        ImGui::ColorEdit4("water", (float *)&waterColor);
        ImGui::SliderFloat("sand", &sandHeight, 0, 0.99);
        ImGui::ColorEdit4("sand", (float *)&sandColor);
        ImGui::ColorEdit4("grass", (float *)&grassColor);
        ImGui::SliderFloat("snow", &snowHeight, 0, 0.99);
        ImGui::ColorEdit4("snow", (float *)&snowColor);
        ImGui::End();
        imgui.end(window);

        if (!(heightMapOptions == prevOptions)) {
            regenHeightMap();
            prevOptions = heightMapOptions;
        }

        window.update();
    }
}
