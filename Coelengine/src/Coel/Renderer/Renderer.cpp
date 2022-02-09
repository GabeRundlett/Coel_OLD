#include <glad/glad.h>
#include "Renderer.hpp"

namespace Coel { namespace Renderer {
    void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
    }
    void clearColor() {
        glClear(GL_COLOR_BUFFER_BIT); //
    }
    void clearDepth() {
        glClear(GL_DEPTH_BUFFER_BIT); //
    }
    void setClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a); //
    }
    void setClearDepth(float v) {
        glClearDepthf(v); //
    }
    void resizeViewport(int32_t x, int32_t y, int32_t w, int32_t h) {
        glViewport(x, y, w, h); //
    }
    void enableBlend(bool mode) {
        if (mode)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void enableCulling(bool mode) {
        if (mode)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }
    void enableDepthTest(bool mode) {
        if (mode)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    void enableWireframe(bool mode) {
        if (mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat) {
        for (const auto &o : model.objects) {
            Coel::send(u_modlMat, &o.modlMat);
            drawIndexed(o.vao, o.indexCount);
        }
    }
    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat, const Uniform<glm::mat4> &u_modlNrmMat) {
        for (const auto &o : model.objects) {
            Coel::send(u_modlMat, &o.modlMat);
            Coel::send(u_modlNrmMat, &o.modlNrmMat);
            drawIndexed(o.vao, o.indexCount);
        }
    }
    void draw(const Model &model, const Uniform<glm::mat4> &u_modlMat, const Uniform<glm::mat4> &u_modlNrmMat, const Coel::Uniform<int> &u_tex, const Coel::Uniform<int> &u_nrm_tex) {
        for (const auto &o : model.objects) {
            Coel::send(u_tex, 0);
            if (o.albedo_textures.size() > 0)
                Coel::bind(*o.albedo_textures[0], 0);
            Coel::send(u_nrm_tex, 1);
            if (o.normal_textures.size() > 0)
                Coel::bind(*o.normal_textures[0], 1);
            Coel::send(u_modlMat, &o.modlMat);
            Coel::send(u_modlNrmMat, &o.modlNrmMat);
            drawIndexed(o.vao, o.indexCount);
        }
    }

    void draw(const Vao &vao, uint32_t count) {
        bind(vao);
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    void drawIndexed(const Vao &vao, uint32_t count) {
        bind(vao);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}} // namespace Coel::Renderer
