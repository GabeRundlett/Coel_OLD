#pragma once

#include <vector>

namespace Minecraft {
    struct QuadMesh {
        struct Vertex {
            float x, y, z, u, v;
        };
        typedef unsigned short Index;
        struct Vertices {
            Vertex a, b, c, d;
        };
        struct Indices {
            Index a1, a2, a3, b1, b2, b3;
        };

        std::vector<Vertices> vertices;
        std::vector<Indices> indices;
        unsigned int vCount{}, iCount{};

        QuadMesh(unsigned int count);
    };
    
    enum QuadNormal {
        Pos,
        Neg,
    };
}
