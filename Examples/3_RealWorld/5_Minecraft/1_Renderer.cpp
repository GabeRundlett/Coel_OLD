#include "0_Resources/1_Renderer/Renderer.hpp"
#include "0_Resources/1_Renderer/Shaders.hpp"

int main() {
    Coel::Window window("Minecraft Example - 1: Renderer");
    Coel::create(window);

    Coel::Renderer::init(batch, MAX_VSIZE, MAX_ISIZE);

    Coel::Shader shader(vertSrc, fragSrc);
    auto u_proj = shader.findMat4("u_proj");
    auto u_view = shader.findMat4("u_view");
    Coel::Texture texture("Assets/dirt.png");
    texture.setMagFilter(Coel::Filter::Nearest);
    Coel::Camera3D cam;
    cam.updatePosition({0, 0, 0});
    cam.updateRotation({0, 0, 0});

    window.onResize = [&cam, &shader, &u_proj, &u_view](Coel::Window &w) {
        cam.updateAspect(w.size);
        shader.send(u_proj, &cam.projMat);
        shader.send(u_view, &cam.viewMat);
    };

    window.onResize(window);

    Coel::Renderer::enableCulling(true);

    while (window.isOpen) {
        Coel::Renderer::clearColor();
        begin();

        submitCube({0, 0, -1});

        shader.bind();
        texture.bind(0);
        flush();
        Coel::update(window);
    }

    Coel::Renderer::deinit(batch);

    Coel::destroy(window);
}
