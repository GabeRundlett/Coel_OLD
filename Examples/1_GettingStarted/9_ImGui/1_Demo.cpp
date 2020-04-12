#include <Coel.hpp>

int main() {
    Coel::Window window({1600, 960}, "ImGui Example");
    Coel::create(window);

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        while (window.isOpen) {
            Coel::Renderer::clearColor();
            imgui.begin();

            ImGui::ShowDemoWindow();

            imgui.end(window);
            Coel::update(window);
        }
    }

    Coel::destroy(window);
}
