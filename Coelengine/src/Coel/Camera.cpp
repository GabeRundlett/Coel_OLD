#include "Camera.hpp"
#include <glm/ext.hpp>

namespace Coel {
    void Camera3D::updateAspect(const glm::ivec2 &size) {
        aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        projMat = glm::perspective(fov, aspect, 0.01f, 200.f);
    }
    void Camera3D::updateFov(const float radians) {
        fov = radians;
        projMat = glm::perspective(fov, aspect, 0.01f, 200.f);
    }
    void Camera3D::updatePosition(const glm::vec3 &p) {
        tranMat = glm::translate(glm::mat4{1}, p);
        viewMat = rotMat * tranMat;
        pos = p;
    }
    void Camera3D::updateRotation(const glm::vec3 &r) {
        rotMat = glm::rotate(glm::rotate(glm::mat4{1}, -r.x, {1, 0, 0}), -r.y, {0, 1, 0});
        viewMat = rotMat * tranMat;
        rot = r;
    }
} // namespace Coel
