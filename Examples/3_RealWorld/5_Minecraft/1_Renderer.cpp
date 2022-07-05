#include "0_Resources/1_Renderer/Renderer.hpp"
#include "0_Resources/1_Renderer/Shaders.hpp"

int main() {
    Coel::Window window("Minecraft Example - 1: Renderer");
    Coel::create(window);

    Coel::Renderer::init(batch, MAX_VSIZE, MAX_ISIZE);

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);
    auto u_proj = Coel::findMat4(shader, "u_proj");
    auto u_view = Coel::findMat4(shader, "u_view");
    Coel::Texture texture;
    Coel::create(texture, "Assets/dirt.png");
    Coel::setMagFilter(texture, Coel::Nearest);
    Coel::Camera3D cam;
    cam.updatePosition({0, 0, 0});
    cam.updateRotation({0, 0, 0});

    window.onResize = [&cam, &shader, &u_proj, &u_view](Coel::Window &w) {
        cam.updateAspect(w.size);

        Coel::bind(shader);
        Coel::send(u_proj, &cam.projMat);
        Coel::send(u_view, &cam.viewMat);
    };

    window.onResize(window);

    Coel::Renderer::enableCulling(true);

    while (window.isOpen) {
        Coel::Renderer::clearColor();
        begin();

        submitCube({0, 0, -1});

        Coel::bind(shader);
        // Coel::send(u_tex, 0);
        flush();
        Coel::update(window);
    }

    Coel::Renderer::deinit(batch);

    Coel::destroy(window);
}
