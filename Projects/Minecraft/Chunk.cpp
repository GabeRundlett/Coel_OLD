#include "Chunk.hpp"

namespace Minecraft {

    Chunk::Chunk() : blocks{100}, mesh{6 * SIZE_X * SIZE_Y * SIZE_Z} {
        generate();
        update();
    }

    void Chunk::generate() {
        for (auto &layerZY : blocks) 
            for (auto &lineZ : layerZY) 
                for (auto &b : lineZ) 
                    b.idX = Block::Dirt, b.idY = Block::Var1;
    }

    void Chunk::update() {
        for (unsigned int zi = 0; zi < SIZE_Z; ++zi)
            for (unsigned int yi = 0; yi < SIZE_Y; ++yi)
                for (unsigned int xi = 0; xi < SIZE_X; ++xi) {
                    Block &b = blocks[zi][yi][xi];

                    if (xi > 0) {
                        if (blocks[zi][yi][xi - 1].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi, (float)yi, (float)zi, 0, 0},
                                {(float)xi + 1, (float)yi, (float)zi, 1, 0},
                                {(float)xi, (float)yi + 1, (float)zi, 0, 1},
                                {(float)xi + 1, (float)yi + 1, (float)zi, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 2),
                                                    QuadMesh::Index(mesh.vCount + 1), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 3)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }
                    if (xi < SIZE_X - 1) {
                        if (blocks[zi][yi][xi + 1].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi, (float)yi, (float)zi + 1, 0, 0},
                                {(float)xi + 1, (float)yi, (float)zi + 1, 1, 0},
                                {(float)xi, (float)yi + 1, (float)zi + 1, 0, 1},
                                {(float)xi + 1, (float)yi + 1, (float)zi + 1, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 3), QuadMesh::Index(mesh.vCount + 2)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }

                    if (yi > 0) {
                        if (blocks[zi][yi - 1][xi].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi, (float)yi, (float)zi, 0, 0},
                                {(float)xi, (float)yi, (float)zi + 1, 1, 0},
                                {(float)xi + 1, (float)yi, (float)zi, 0, 1},
                                {(float)xi + 1, (float)yi, (float)zi + 1, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 2),
                                                    QuadMesh::Index(mesh.vCount + 1), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 3)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }
                    if (yi < SIZE_Y - 1) {
                        if (blocks[zi][yi + 1][xi].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi, (float)yi + 1, (float)zi, 0, 0},
                                {(float)xi, (float)yi + 1, (float)zi + 1, 1, 0},
                                {(float)xi + 1, (float)yi + 1, (float)zi, 0, 1},
                                {(float)xi + 1, (float)yi + 1, (float)zi + 1, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 3), QuadMesh::Index(mesh.vCount + 2)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }

                    if (zi > 0) {
                        if (blocks[zi - 1][yi][xi].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi, (float)yi, (float)zi, 0, 0},
                                {(float)xi, (float)yi + 1, (float)zi, 1, 0},
                                {(float)xi, (float)yi, (float)zi + 1, 0, 1},
                                {(float)xi, (float)yi + 1, (float)zi + 1, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 2),
                                                    QuadMesh::Index(mesh.vCount + 1), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 3)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }
                    if (zi < SIZE_Z - 1) {
                        if (blocks[zi + 1][yi][xi].idX > Block::Transparent) {
                            mesh.vertices.push_back({
                                {(float)xi + 1, (float)yi, (float)zi, 0, 0},
                                {(float)xi + 1, (float)yi + 1, (float)zi, 1, 0},
                                {(float)xi + 1, (float)yi, (float)zi + 1, 0, 1},
                                {(float)xi + 1, (float)yi + 1, (float)zi + 1, 1, 1},
                            });
                            mesh.indices.push_back({QuadMesh::Index(mesh.vCount + 0), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 2), QuadMesh::Index(mesh.vCount + 1),
                                                    QuadMesh::Index(mesh.vCount + 3), QuadMesh::Index(mesh.vCount + 2)});
                            mesh.vCount += 4, mesh.iCount += 6;
                        }
                    }
                }
    }
}
