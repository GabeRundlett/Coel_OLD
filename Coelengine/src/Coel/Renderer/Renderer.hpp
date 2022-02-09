#pragma once

#include <Coel/Model.hpp>
#include <Coel/Renderer/Shader.hpp>

namespace Coel { namespace Renderer {
    void clear();
    void clearColor();
    void clearDepth();
    void setClearColor(float r, float g, float b, float a);
    void setClearDepth(float v);
    void resizeViewport(int32_t l, int32_t t, int32_t r, int32_t b);
    void enableBlend(bool mode);
    void enableCulling(bool mode);
    void enableDepthTest(bool mode);
    void enableWireframe(bool mode);
    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat);
    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat, const Uniform<glm::mat4> &u_modlNrmMat);
    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat, const Uniform<glm::mat4> &u_modlNrmMat, const Coel::Uniform<int> &u_tex, const Coel::Uniform<int> &u_nrm_tex);
    void draw(const Vao &vao, uint32_t count);
    void drawIndexed(const Vao &vao, uint32_t count);
}} // namespace Coel::Renderer
