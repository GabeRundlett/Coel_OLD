#include <Coel.hpp>
#ifdef WIN32
#include <Windows.h>
static inline void s_sleep(std::size_t ms) {
    Sleep(ms);
}
#else
#include <unistd.h>
static inline void s_sleep(std::size_t ms) {
    usleep(ms * 1000);
}
#endif
#include <iostream>

#include <Coel/Renderers/ImGui/ImGuiTextEditor.hpp>

void dockspace() {
    static bool open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |=
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Dock Menu")) {
            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                                (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
                                (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            ImGui::Separator();
            if (ImGui::MenuItem("Close DockSpace", NULL, false))
                open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

int main() {
    Coel::Window window{{1440, 720}, "Cellular Automata"};
    Coel::create(window);
    constexpr glm::ivec2 gridSize = {32, 32};

    const char *const vertSrc = R"(
#version 440
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex;
out vec2 v_tex;
void main() {
    v_tex = a_tex;
    gl_Position = vec4(a_pos, 0, 1);
})";

    const char *const quadFragSrc = R"(
#version 440
in vec2 v_tex;
out vec4 color;
uniform sampler2D u_tex;
void main() {
    vec4 tex = texture(u_tex, v_tex);
    color = vec4(tex.r);
})";

    const char *const startGridFragSrc = R"(
#version 440
in vec2 v_tex;
out vec4 color;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))*
    43758.5453123);
}

void main() {
    float r = random(v_tex);
    if (r > 0.9) color = vec4(1);
    else color = vec4(0);
})";

    const char *const automataVertSrc = R"(
#version 440
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex;
out vec2 v_tex_5rj9aiBR[9];
uniform vec2 tileSize;
void main() {
    gl_Position = vec4(a_pos, 0, 1);
    for (int yi = -1; yi < 2; yi++) {
        for (int xi = -1; xi < 2; xi++) {
            const int index = (xi + 1) + (yi + 1) * 3;
            v_tex_5rj9aiBR[index] = a_tex + vec2(xi, yi) * tileSize;
        }   
    }
})";
    const char *const automataFragSrc_start = R"(
#version 440
in vec2 v_tex_5rj9aiBR[9];
out vec4 color_5rj9aiBR;
uniform sampler2D u_tex_5rj9aiBR;

struct automaton_5rj9aiBR {
    int kernel_id;
    bool neighbors[9];

    bool is_alive;
    int neighbor_count;
} otto;

)";
    const char *const conwaysGameOfLife = R"(
bool automata_main() {
    if (otto.is_alive) {
        if (otto.neighbor_count < 2) 
            return false;
        if (otto.neighbor_count > 3) 
            return false;
        return true;
    } else {
        if (otto.neighbor_count == 3) 
            return true;
        return false;
    }
})";
    const char *const automataFragSrc_end = R"(
    
void main() {
    otto.kernel_id = 0;
    otto.neighbor_count = 0;
    otto.is_alive = false;
    for (int i = 0; i < 9; i++) {
        vec4 result = texture(u_tex_5rj9aiBR, v_tex_5rj9aiBR[i]);
        if (result.r > 0.9){
            otto.neighbors[i] = true;
            otto.kernel_id |= 1 << i;
            if (i == 4) otto.is_alive = true;
            else otto.neighbor_count++;
        } else {
            otto.neighbors[i] = false;
        }
    }
    color_5rj9aiBR = vec4(automata_main());
})";

    Coel::Renderer::Quad2d quadRenderer;
    Coel::Shader quadShader;
    Coel::create(quadShader, vertSrc, quadFragSrc);
    auto u_tex = Coel::findInt(quadShader, "u_tex");

    glm::vec2 tileSize = {1.f / gridSize.x, 1.f / gridSize.y};

    Coel::Shader startGridShader;
    Coel::create(startGridShader, vertSrc, startGridFragSrc);
    auto u_copyGridTex = Coel::findInt(startGridShader, "u_tex");

    std::string automataFragSrc;
    Coel::Shader automataShader;
    automataShader.id = 0;
    Coel::Uniform<int> u_automataTex;
    Coel::Uniform<glm::vec2> u_tileSize;

    const auto create_automata_program = [&](const std::string &program) {
        if (!automataShader.id)
            Coel::destroy(automataShader);
        std::string tempFragSrc = std::string(automataFragSrc_start) + program + automataFragSrc_end;
        Coel::create(automataShader, automataVertSrc, tempFragSrc.c_str());
        if (automataShader.id) {
            automataFragSrc = tempFragSrc;
            u_automataTex = Coel::findInt(automataShader, "u_tex_5rj9aiBR");
            u_tileSize = Coel::findFloat2(automataShader, "tileSize");
        } else {
            Coel::create(automataShader, automataVertSrc, automataFragSrc.c_str());
            std::cout << "Failed to compile shader\n";
        }
    };

    create_automata_program(conwaysGameOfLife);

    Coel::Texture gridTextures[2];
    Coel::create(gridTextures[0], gridSize, Coel::RED, nullptr);
    Coel::create(gridTextures[1], gridSize, Coel::RED, nullptr);

    for (auto &tex : gridTextures) {
        Coel::setMinFilter(tex, Coel::Nearest);
        Coel::setMagFilter(tex, Coel::Nearest);
    }

    Coel::Rbo fboDepthRbo;
    Coel::create(fboDepthRbo, gridSize);

    int i = 0;
    Coel::Fbo gridFbos[2];
    for (auto &fbo : gridFbos) {
        Coel::create(fbo, gridSize);
        Coel::attach(fbo, {gridTextures[i]});
        Coel::attach(fbo, fboDepthRbo);
        ++i;
    }

    const auto reset_automata = [&] {
        Coel::bind(startGridShader);
        Coel::bind(gridFbos[0]);
        quadRenderer.draw();
        Coel::bind(gridFbos[1]);
        quadRenderer.draw();
    };
    reset_automata();

    const auto automata_from_to = [&](int i1, int i2) {
        Coel::bind(automataShader);
        Coel::bind(gridFbos[i2]);
        Coel::send(u_automataTex, 0);
        Coel::bind(gridTextures[i1], 0);
        Coel::send(u_tileSize, &tileSize);
        Coel::Renderer::setClearColor(0, 0, 0, 1);
        Coel::Renderer::clearColor();
        quadRenderer.draw();
    };

    i = 0;
    const auto run_automata = [&] {
        automata_from_to(i, !i);
        i = !i;
    };

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        TextEditor editor;
        auto lang = TextEditor::LanguageDefinition::GLSL();
        editor.SetLanguageDefinition(lang);
        editor.SetText(conwaysGameOfLife);

        window.onKey = [&](Coel::Window &w) {
            switch (w.key.action) {
            case Coel::Action::Press:
                switch (w.key.code) {
                case Coel::Key::R:
                case Coel::Key::F5:
                    reset_automata();
                case Coel::Key::S:
                    if (w.key.mods & Coel::Mod::Control)
                        create_automata_program(editor.GetText());
                    break;
                case Coel::Key::Up: run_automata(); break;
                }
                break;
            }
        };
        bool sim = false;

        while (window.isOpen) {
            if (sim)
                run_automata();
            Coel::bind(window.fbo);
            Coel::Renderer::clearColor();
            imgui.begin();

            dockspace();

            ImGui::Begin("Game Of Life");
            auto size = gridTextures[i].size;
            auto viewport_size = ImGui::GetWindowSize();
            viewport_size.x -= 18, viewport_size.y -= 18;

            ImGui::Image((void *)(intptr_t)gridTextures[i].id, viewport_size);
            ImGui::End();

            auto cpos = editor.GetCursorPosition();

            ImGui::Begin("Game Rule Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

            if (ImGui::Button("APPLY", ImVec2(200, 50))) {
                create_automata_program(editor.GetText());
            }
            ImGui::Checkbox("Simulate", &sim);

            ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
                        editor.IsOverwrite() ? "Ovr" : "Ins",
                        editor.CanUndo() ? "*" : " ",
                        editor.GetLanguageDefinition().mName.c_str(), "Game Rules");
            editor.Render("Game Rule Editor");
            ImGui::End();

            imgui.end(window);
            s_sleep(10);
            Coel::update(window);
        }
    }
}
