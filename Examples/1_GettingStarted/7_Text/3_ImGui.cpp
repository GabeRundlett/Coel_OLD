#include <Coel.hpp>

int main() {
    Coel::Window window({1400, 800}, "ImGui Example");
    Coel::create(window);

    Coel::Renderer::ImGuiRenderer imgui(window);
    bool mySetting = false;

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        imgui.begin();
        ImGui::Image(0, ImVec2(256, 256));
        imgui.end(window);

        Coel::update(window);
    }
}
