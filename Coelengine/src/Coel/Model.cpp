#include "Model.hpp"
#include "Renderer/Renderer.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Coel {
    Texture defaultTex;
    Texture defaultNormalTex;
    bool defaultsCreated = false;

    static void loadTextures(Model::Object &object, aiMaterial *mat, aiTextureType type, std::string rootdir) {
        auto &textures = [](Model::Object &object, aiTextureType type) -> std::vector<TextureRef> & {
            switch (type) {
            case aiTextureType_DIFFUSE:
                return object.albedo_textures;
            case aiTextureType_NORMALS:
                return object.normal_textures;
            default:
                return object.albedo_textures;
            }
        }(object, type);
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            textures.push_back(load((rootdir + '/' + str.C_Str()).c_str()));
        }
        if (textures.size() == 0) {
            if (!defaultsCreated) {
                defaultsCreated = true;
                constexpr glm::ivec2 defaultTexSize{64, 64};

                uint32_t defaultTexCol[defaultTexSize.x * defaultTexSize.y];
                for (int yi = 0; yi < defaultTexSize.y; ++yi) {
                    for (int xi = 0; xi < defaultTexSize.x; ++xi) {
                        int i = xi + yi * defaultTexSize.x;
                        if ((xi + yi) % 2 == 0) {
                            defaultTexCol[i] = 0xffff44ff;
                        } else {
                            defaultTexCol[i] = 0xff662266;
                        }
                    }
                }
                create(defaultTex, {defaultTexSize.x, defaultTexSize.y}, TextureFormat::RGBA, reinterpret_cast<uint8_t *>(defaultTexCol));
                setMagFilter(defaultTex, TextureFilter::Nearest);

                constexpr glm::ivec2 defaultTexNrmSize{1, 1};
                uint32_t defaultTexNrmCol[defaultTexSize.x * defaultTexSize.y];
                for (int yi = 0; yi < defaultTexSize.y; ++yi) {
                    for (int xi = 0; xi < defaultTexSize.x; ++xi) {
                        int i = xi + yi * defaultTexSize.x;
                        defaultTexNrmCol[i] = 0xffff7f7f;
                    }
                }
                create(defaultNormalTex, {defaultTexNrmSize.x, defaultTexNrmSize.y}, TextureFormat::RGBA, reinterpret_cast<uint8_t *>(defaultTexNrmCol));
                setMagFilter(defaultNormalTex, TextureFilter::Nearest);
            }
            switch (type) {
            case aiTextureType_DIFFUSE:
                textures.push_back(&defaultTex);
                break;
            case aiTextureType_NORMALS:
                textures.push_back(&defaultNormalTex);
                break;
            default: break;
            }
        }
    }

    static void processNode(Model &model, aiNode *node, const aiScene *scene, std::string rootdir) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            Model::Object tempObj;
            tempObj.modlMat = *reinterpret_cast<glm::mat4 *>(&node->mTransformation);
            tempObj.modlMat = glm::transpose(tempObj.modlMat);
            tempObj.modlNrmMat = glm::inverse(glm::transpose(tempObj.modlMat));

            std::vector<Model::Vertex> vertices;
            std::vector<unsigned int> indices;

            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                Model::Vertex vertex;
                vertex.pos = *reinterpret_cast<glm::vec3 *>(mesh->mVertices + j);
                vertex.nrm = *reinterpret_cast<glm::vec3 *>(mesh->mNormals + j);
                vertex.tex = *reinterpret_cast<glm::vec3 *>(mesh->mTextureCoords[0] + j);
                vertices.push_back(vertex);
            }
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    if (face.mNumIndices != 3)
                        std::cout << "index count: " << face.mNumIndices << '\n';
                    indices.push_back(face.mIndices[k]);
                }
            }

            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            loadTextures(tempObj, material, aiTextureType_DIFFUSE, rootdir);
            loadTextures(tempObj, material, aiTextureType_SPECULAR, rootdir);
            loadTextures(tempObj, material, aiTextureType_HEIGHT, rootdir);
            loadTextures(tempObj, material, aiTextureType_AMBIENT, rootdir);
            loadTextures(tempObj, material, aiTextureType_NORMALS, rootdir);

            create(tempObj.vao);
            create(tempObj.vbo, vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Model::Vertex)));
            create(tempObj.ibo, indices.data(), static_cast<uint32_t>(indices.size() * sizeof(unsigned int)));
            link(tempObj.vao, tempObj.vbo);
            tempObj.indexCount = static_cast<uint32_t>(indices.size());
            model.objects.push_back({tempObj});
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            processNode(model, node->mChildren[i], scene, rootdir);
    }

    void open(Model &model, std::string filepath) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        auto last_slash = filepath.rfind('/');
        processNode(model, scene->mRootNode, scene, filepath.substr(0, last_slash));
    }
} // namespace Coel
