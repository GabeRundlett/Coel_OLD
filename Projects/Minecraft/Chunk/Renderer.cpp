#include "Renderer.hpp"

#include <memory>
#include <algorithm>
#include <iterator>

namespace Minecraft {
    Renderer::Renderer()
        : vbo(nullptr, sizeof(QuadMesh::Vertex) * VERTEX_COUNT, {{Coel::Element::F32, 3}, {Coel::Element::F32, 2}}),
          ibo(nullptr, sizeof(QuadMesh::Index) * INDEX_COUNT), shader(vertSrc, geomSrc, fragSrc),
          texture("C:/Dev/Coel/Projects/Minecraft/Assets/Atlas.png") {
        Coel::Renderer::Command::setClearColor(0.694f, 0.808f, 1, 1);
        vao.add(vbo);
        texture.bind(0);
        begin();
    }
    void Renderer::begin() {
        ibo.open(&quadIbo);
        vbo.open(&quadVbo);

        indexCount = 0, vertexCount = 0;
    }
    void Renderer::end() {
        vbo.close();
        ibo.close();
    }
    void Renderer::flush() {
        end();
        vao.drawIndexed(indexCount);
        begin();
    }
    void Renderer::submitPrism(const Math::Vec3 &pos, const Math::Vec3 &size) {
        // submitQuadXY<Neg>(pos, {size.x, size.y}, {0, 0});
        // submitQuadXY<Pos>({pos.x, pos.y, pos.z + size.z}, {size.x, size.y}, {0, 0});
        // 
        // submitQuadXZ<Neg>(pos, {size.x, size.z}, {0, 0});
        // submitQuadXZ<Pos>({pos.x, pos.y + size.y, pos.z}, {size.x, size.z}, {0, 0});
        // 
        // submitQuadYZ<Neg>(pos, {size.y, size.z}, {0, 0});
        // submitQuadYZ<Pos>({pos.x + size.x, pos.y, pos.z}, {size.y, size.z}, {0, 0});
    }
    void Renderer::submitChunk(Chunk &c) { 
        flush();

    }
    /*
    
        auto vbeg = mesh.vertices.begin();
        auto ibeg = mesh.indices.begin();
        auto alreadySentCount = 0;

        for (unsigned int i = 0; i < 10; ++i) {
            const auto vleft = (VERTEX_COUNT - vertexCount) / 4,
                       ileft = (INDEX_COUNT - indexCount) / 6, left = vleft < ileft ? vleft : ileft;
            const auto vend = vbeg + left;
            const auto iend = ibeg + left;

            if (vend < mesh.vertices.end() && iend < mesh.indices.end()) {
                for (unsigned int ix = alreadySentCount; ix < alreadySentCount + left; ++ix) {
                    auto &index = mesh.indices[ix];
                    index.a1 += indexCount - alreadySentCount * 4;
                    index.a2 += indexCount - alreadySentCount * 4;
                    index.a3 += indexCount - alreadySentCount * 4;
                    index.b1 += indexCount - alreadySentCount * 4;
                    index.b2 += indexCount - alreadySentCount * 4;
                    index.b3 += indexCount - alreadySentCount * 4;
                }

                std::copy(vbeg, vend, quadVbo);
                std::copy(ibeg, iend, quadIbo);

            } else {
                const auto lleft = std::distance(vbeg, mesh.vertices.end());
                for (unsigned int ix = alreadySentCount; ix < alreadySentCount + lleft; ++ix) {
                    auto &index = mesh.indices[ix];
                    index.a1 += indexCount - alreadySentCount * 4;
                    index.a2 += indexCount - alreadySentCount * 4;
                    index.a3 += indexCount - alreadySentCount * 4;
                    index.b1 += indexCount - alreadySentCount * 4;
                    index.b2 += indexCount - alreadySentCount * 4;
                    index.b3 += indexCount - alreadySentCount * 4;
                }

                std::copy(vbeg, mesh.vertices.end(), quadVbo);
                std::copy(ibeg, mesh.indices.end(), quadIbo);
                break;
            }

            quadVbo += left * 4;
            quadIbo += left * 6;
            alreadySentCount += left;
            vbeg = vend, ibeg = iend;
        }*/
    void Renderer::updateCamera(const Coel::Camera3D& cam) {
        auto viewMat = Math::Mat4::identity();
        viewMat *= cam.projMat;
        viewMat *= cam.rotMat;
        viewMat *= cam.tranMat;
        shader.sendMat4("viewMat", &viewMat);
    }
    
}
