#include <Coel.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Shaders.hpp"

struct Vertex {
    glm::vec3 pos, nrm;
    glm::vec2 tex;
    glm::vec4 col;
};

void loadObj(const char *const filepath, std::vector<Vertex> &data) {
    std::ifstream modelFile(filepath);
    struct TriIndex {
        struct V {
            union {
                struct {
                    int pos, tex, nrm;
                };
                int data[3];
            };
        } v[3];
    };
    if (modelFile.is_open()) {
        std::string line;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
        std::vector<TriIndex> triangles;
        std::size_t triCount = 0;
        while (std::getline(modelFile, line)) {
            if (line[0] == 'v') {
                if (line[1] == ' ') { // Position
                    line = line.substr(2);
                    positions.push_back({});
                    auto &pos = positions.back();
                    for (unsigned int i = 0; i < 3; ++i) {
                        std::size_t termEnd;
                        pos[i] = std::stof(line, &termEnd);
                        line = line.substr(termEnd);
                    }
                } else if (line[1] == 't') { // Texture Coord
                    line = line.substr(3);
                    texCoords.push_back({});
                    auto &tex = texCoords.back();
                    for (unsigned int i = 0; i < 2; ++i) {
                        std::size_t termEnd;
                        tex[i] = std::stof(line, &termEnd);
                        line = line.substr(termEnd);
                    }
                } else if (line[1] == 'n') { // Normal
                    line = line.substr(3);
                    normals.push_back({});
                    auto &nrm = normals.back();
                    for (unsigned int i = 0; i < 3; ++i) {
                        std::size_t termEnd;
                        nrm[i] = std::stof(line, &termEnd);
                        line = line.substr(termEnd);
                    }
                }
            } else if (line[0] == 'f') { // Tri
                line = line.substr(2);
                triangles.push_back({});
                for (unsigned int i = 0; i < 3; ++i) {
                    std::size_t termEnd;
                    auto a = std::stoi(line, &termEnd) - 1;
                    triangles[triCount].v[0].data[i] = a;
                    line = line.substr(termEnd + 1);
                }
                for (unsigned int i = 0; i < 3; ++i) {
                    std::size_t termEnd;
                    auto a = std::stoi(line, &termEnd) - 1;
                    triangles[triCount].v[1].data[i] = a;
                    line = line.substr(termEnd + 1);
                }
                for (unsigned int i = 0; i < 3; ++i) {
                    std::size_t termEnd;
                    auto a = std::stoi(line, &termEnd) - 1;
                    triangles[triCount].v[2].data[i] = a;
                    if (i < 2) line = line.substr(termEnd + 1);
                }
                ++triCount;
            }
        }

        data.reserve(triCount * 3);
        for (auto &t : triangles) {
            for (auto &v : t.v) {
                data.push_back({
                    positions[v.pos],
                    normals[v.nrm],
                    texCoords[v.tex],
                    {1, 0, 1, 1},
                });
                // std::cout << v.pos << ", " << v.nrm << ", " << v.tex << "  ";
            }
            // std::cout << '\n';
        }
    }
}

int main() {
    Coel::Window window(1400, 1000, "Simple OBJ loader example");
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Texture texture("C:/Dev/Coel/Assets/face.png");

    auto u_mvp = shader.findMat4("u_mvp");
    auto mat = glm::perspective(glm::radians(90.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
    mat = glm::translate(mat, {0, 0, 5});

    std::vector<Vertex> mesh;
    loadObj("C:/Dev/Coel/Assets/stall.obj", mesh);

    Coel::Vao vao;
    Coel::Vbo vbo(mesh.data(), mesh.size() * sizeof(Vertex),
                  {{Coel::Element::F32, 3}, {Coel::Element::F32, 3}, {Coel::Element::F32, 2}, {Coel::Element::F32, 4}});

    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::setClearColor(0.1f, 0.1f, 0.1f, 1.f);

    while (window.isOpen()) {
        Coel::Renderer::clear();

        shader.bind();
        // shader.send(u_mvp, &mat);
        texture.bind(0);
        vao.draw(mesh.size());

        window.update();
    }
}