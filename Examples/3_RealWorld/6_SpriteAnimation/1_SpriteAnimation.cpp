#include <Coel.hpp>
int main() {
    Coel::Window window({800, 800}, "Platformer Example");
    Coel::SpriteRenderer spriteRenderer;

    Coel::create(window);
    Coel::init(spriteRenderer);
    Coel::Renderer::setClearColor(0.24, 0.24, 0.32, 1);
    Coel::Renderer::enableBlend(true);

    Coel::Texture spriteSheet;
    Coel::create(spriteSheet, "Assets/Textures/sheet.png");
    Coel::setMagFilter(spriteSheet, Coel::Nearest);
    Coel::setWrapMode(spriteSheet, Coel::ClampToEdge);

    glm::vec2 texpos = {0, 0}, texsize = {0.25, 0.25};
    int offset = 0;

    {
        Coel::Renderer::ImGuiRenderer imgui(window);
        while (window.isOpen) {
            Coel::Renderer::clearColor();

            spriteRenderer.begin();
            spriteRenderer.submit({0, 0, 2, 2}, {texpos.x + texsize.x * offset, texpos.y, texsize.x, texsize.y}, spriteSheet);
            spriteRenderer.flush();

            imgui.begin();
            ImGui::Begin("Controls");
            ImGui::Text("First Frame");
            ImGui::SliderFloat2("coordinates", (float *)&texpos, 0, 1);
            ImGui::SliderFloat2("dimensions", (float *)&texsize, 0, 1);
            ImGui::SliderInt("offset", &offset, 0, 3);
            ImGui::End();
            imgui.end(window);

            Coel::update(window);
        }
    }
    Coel::destroy(spriteSheet);
    Coel::deinit(spriteRenderer);
    Coel::destroy(window);
}