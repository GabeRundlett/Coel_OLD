#pragma once

#include <Coel.hpp>

#include "Chunk.hpp"
#include "Shaders.hpp"

namespace Minecraft {
    namespace Chunk {
        //
    }
    class Renderer {
      public:
        static inline constexpr unsigned int COUNT = 8000, VERTEX_COUNT = COUNT * 8, INDEX_COUNT = COUNT * 18;

        Coel::Vao vao;
        Coel::Vbo vbo;
        Coel::Ibo ibo;
        Coel::Shader shader;
        Coel::Texture texture;

        QuadMesh::Vertices *quadVbo = nullptr;
        QuadMesh::Indices *quadIbo = nullptr;
        unsigned int vertexCount = 0, indexCount = 0;

        Renderer();

        void begin();
        void end();
        void flush();
        /*
        inline void beginQuadSubmit() {
            if (indexCount + 18 > COUNT) flush();
        }
        template <QuadNormal Normal> inline void endQuadSubmit() {
            if constexpr (Normal == Pos) {
                *quadIbo = {QuadMesh::Index(vertexCount + 0), QuadMesh::Index(vertexCount + 1), QuadMesh::Index(vertexCount + 2),
                            QuadMesh::Index(vertexCount + 1), QuadMesh::Index(vertexCount + 3), QuadMesh::Index(vertexCount + 2)};
            } else {
                *quadIbo = {QuadMesh::Index(vertexCount + 0), QuadMesh::Index(vertexCount + 2), QuadMesh::Index(vertexCount + 1),
                            QuadMesh::Index(vertexCount + 1), QuadMesh::Index(vertexCount + 2), QuadMesh::Index(vertexCount + 3)};
            }
            vertexCount += 4, indexCount += 6;
            ++quadVbo, ++quadIbo;
        }
        template <QuadNormal Normal>
        inline void submitQuadXY(const Math::Vec3 &pos, const Math::Vec2 &size, const Math::Vec2 &tex) {
            beginQuadSubmit();
            *quadVbo = {
                {pos.x, pos.y, pos.z, tex.x, tex.y},
                {pos.x + size.x, pos.y, pos.z, tex.x + size.x, tex.y},
                {pos.x, pos.y + size.y, pos.z, tex.x, tex.y + size.y},
                {pos.x + size.x, pos.y + size.y, pos.z, tex.x + size.x, tex.y + size.y},
            };
            endQuadSubmit<Normal>();
        }
        template <QuadNormal Normal>
        inline void submitQuadXZ(const Math::Vec3 &pos, const Math::Vec2 &size, const Math::Vec2 &tex) {
            beginQuadSubmit();
            *quadVbo = {
                {pos.x, pos.y, pos.z, tex.x, tex.y},
                {pos.x, pos.y, pos.z + size.y, tex.x, tex.y + size.y},
                {pos.x + size.x, pos.y, pos.z, tex.x + size.x, tex.y},
                {pos.x + size.x, pos.y, pos.z + size.y, tex.x + size.x, tex.y + size.y},
            };
            endQuadSubmit<Normal>();
        }
        template <QuadNormal Normal>
        inline void submitQuadYZ(const Math::Vec3 &pos, const Math::Vec2 &size, const Math::Vec2 &tex) {
            beginQuadSubmit();
            *quadVbo = {
                {pos.x, pos.y, pos.z, tex.x, tex.y},
                {pos.x, pos.y + size.x, pos.z, tex.x + size.x, tex.y},
                {pos.x, pos.y, pos.z + size.y, tex.x, tex.y + size.y},
                {pos.x, pos.y + size.x, pos.z + size.y, tex.x + size.x, tex.y + size.y},
            };
            endQuadSubmit<Normal>();
        }
        */
        void submitPrism(const Math::Vec3 &pos, const Math::Vec3 &size);
        void submitChunk(Chunk &c);
        void updateCamera(const Coel::Camera3D &cam);
    };
} // namespace Minecraft
