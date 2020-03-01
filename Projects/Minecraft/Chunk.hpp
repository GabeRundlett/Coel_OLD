#pragma once

#include <Coel.hpp>
#include "Mesh.hpp"

namespace Minecraft {
    struct BlockMeshVisibility {
        unsigned char posX : 1, negX : 1, posY : 1, negY : 1, posZ : 1, negZ : 1;
    };

    struct Block {
        enum IdX {
            None,
            Air,

            Transparent,

            Dirt,
            Stone,
        };
        enum IdY {
            Var1,
            Var2,
        };

        unsigned char idX, idY;
    };

    struct Chunk {
        static constexpr unsigned int SIZE_X = 16, SIZE_Y = 16, SIZE_Z = 16;
        Block blocks[SIZE_Z][SIZE_Y][SIZE_X];
        QuadMesh mesh;

        Chunk();

        void generate();
        void update();
    };
} // namespace Minecraft
