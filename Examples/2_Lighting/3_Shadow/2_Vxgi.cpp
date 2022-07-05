#include <Coel.hpp>
#include <iostream>

#include "0_Player.hpp"

int main() {
    Coel::Window window("Simple Shadow Mapping Example");
    Coel::create(window);

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        const char *const vertSrc = R"(
        #version 450 core
        layout (location = 0) in vec3 a_pos;
        layout (location = 1) in vec3 a_nrm;
        layout (location = 2) in vec2 a_tex;
        out vec3 v_pos;
        out vec3 v_lightSpacePos;
        out vec3 v_nrm;
        out vec2 v_tex;
        uniform mat4 u_projMat;
        uniform mat4 u_viewMat;
        uniform mat4 u_lightProjMat;
        uniform mat4 u_lightViewMat;
        uniform mat4 u_modlMat;
        uniform mat4 u_modlNrmMat;
        void main() {
            vec4 worldPos = u_modlMat * vec4(a_pos, 1);
            vec4 worldNrm = u_modlNrmMat * vec4(a_nrm, 1);
            v_pos = worldPos.xyz;
            vec4 lightSpacePos = u_lightProjMat * u_lightViewMat * worldPos;
            v_lightSpacePos = lightSpacePos.xyz / lightSpacePos.w / 2 + 0.5;
            v_nrm = worldNrm.xyz, v_tex = vec2(a_tex.x, a_tex.y);
            gl_Position = u_projMat * u_viewMat * worldPos;
        })";

        const char *const fragSrc = R"(
        #version 450 core
        in vec3 v_pos;
        in vec3 v_lightSpacePos;
        in vec3 v_nrm;
        in vec2 v_tex;
        out vec4 frag_color;
        uniform sampler2D u_tex;
        uniform sampler2D u_shadowMap;
        uniform vec3 u_lightDir;
        uniform vec3 u_sunCol;
        uniform vec3 u_skyCol;
        uniform vec3 u_camPos;
        uniform float u_sunContrib;
        uniform float u_skyContrib;

        float shadowCalc(in float dotLightNrm) {
            vec3 pos = v_lightSpacePos;
            if (pos.z > 1) pos.z = 1;
            
            float factor = 0.f;
            float bias = 0.001f; // max(0.05 * (dotLightNrm), 0.005);
            vec2 texelSize = 1.f / textureSize(u_shadowMap, 0);

            for (int y = 0; y <= 0; ++y) {
                for (int x = 0; x <= 0; ++x) {
                    float depth = texture(u_shadowMap, pos.xy + vec2(float(x), float(y)) * texelSize).r;
                    if (depth + bias < pos.z)
                        factor += 1.0f / (1 * 1);
                }
            }
            return 1 - factor;
        }

        vec3 filmicToneMapping(vec3 color) {
            color = max(vec3(0.), color - vec3(0.004));
            color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
            return color;
        }

        void main() {
            vec3 lightDir = normalize(u_lightDir);
            vec3 v_col = texture(u_tex, v_tex).rgb;
            // v_col = pow(max(v_col, 0), vec3(2.2));
            vec3 lightCol = vec3(1);
            vec3 nrm = normalize(v_nrm);

            float dotLightNrm = dot(lightDir, nrm);
            float diff = max(dotLightNrm, 0);
            float shadow = shadowCalc(dotLightNrm);
            diff *= shadow;
            diff = max(diff, 0.0) * 1;

            vec3 posToCam = normalize(v_pos + u_camPos);
            vec3 reflPosToCam = reflect(-posToCam, nrm);
            vec3 posToLight = lightDir;
            vec3 spec = u_sunContrib * u_sunCol * shadow * max(pow(dot(-reflPosToCam, posToLight), 100), 0);

            vec3 color = (u_sunCol * diff * u_sunContrib + u_skyCol * u_skyContrib) * v_col + spec;
            color = filmicToneMapping(color);

            frag_color = vec4(color, 1);
        })";

        Coel::Shader shader;
        Coel::create(shader, vertSrc, fragSrc);

        auto u_projMat = Coel::findMat4(shader, "u_projMat");
        auto u_viewMat = Coel::findMat4(shader, "u_viewMat");
        auto u_lightProjMat = Coel::findMat4(shader, "u_lightProjMat");
        auto u_lightViewMat = Coel::findMat4(shader, "u_lightViewMat");
        auto u_modlMat = Coel::findMat4(shader, "u_modlMat");
        auto u_modlNrmMat = Coel::findMat4(shader, "u_modlNrmMat");
        glm::mat4 projMat, viewMat;

        Player player{};
        player.pos = {0, -5, 0};
        player.rot = {glm::radians(-30.0f), glm::radians(90 - 65.0f), 0.0f};
        Camera camera{};
        bool is_paused = true;

        auto toggle_pause = [&]() {
            double center_x = static_cast<double>(window.size.x) / 2;
            double center_y = static_cast<double>(window.size.y) / 2;
            Coel::cursorTo(window, {center_x, center_y});
            if (is_paused) {
                Coel::cursorMode(window, Coel::CursorMode::Hidden);
                is_paused = false;
            } else {
                Coel::cursorMode(window, Coel::CursorMode::Normal);
                is_paused = true;
            }
        };

        toggle_pause();

        window.onKey = [&](Coel::Window &window) {
            if (window.key.code == input::keybinds::TOGGLE_PAUSE && window.key.action == GLFW_PRESS)
                toggle_pause();
            if (!is_paused) {
                player.on_key(window.key.code, window.key.action);
            }
        };
        window.onMouseMove = [&](Coel::Window &window) {
            double center_x = static_cast<double>(window.size.x) / 2;
            double center_y = static_cast<double>(window.size.y) / 2;

            auto x = window.mouse.pos.x;
            auto y = window.mouse.pos.y;
            auto dx = x - center_x;
            auto dy = y - center_y;
            if (!is_paused) {
                player.on_mouse_move(dx, -dy);
                Coel::cursorTo(window, {center_x, center_y});
            }
        };

        auto u_tex = Coel::findInt(shader, "u_tex");
        auto u_nrm_tex = Coel::findInt(shader, "u_nrm_tex");

        Coel::Model model;
        Coel::open(model, "Assets/Objects/frog/scene.gltf");

        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);

        Coel::Fbo shadowMapFbo;
        Coel::create(shadowMapFbo, {512 * 4, 512 * 4});
        Coel::Texture shadowMapTexture;
        Coel::create(shadowMapTexture, shadowMapFbo.size, Coel::TextureFormat::Depth, nullptr);
        Coel::setMagFilter(shadowMapTexture, Coel::TextureFilter::Nearest);
        Coel::attach(shadowMapFbo, {shadowMapTexture});
        auto u_shadowMap = Coel::findInt(shader, "u_shadowMap");
        auto u_lightDir = Coel::findFloat3(shader, "u_lightDir");
        auto u_sunCol = Coel::findFloat3(shader, "u_sunCol");
        auto u_skyCol = Coel::findFloat3(shader, "u_skyCol");
        auto u_camPos = Coel::findFloat3(shader, "u_camPos");
        auto u_sunContrib = Coel::findFloat(shader, "u_sunContrib");
        auto u_skyContrib = Coel::findFloat(shader, "u_skyContrib");

        float sun_angle = 45.0f, sun_elevation = 45.0f;
        glm::vec3 sunCol{1.0f, 0.8f, 0.6f};
        glm::vec3 skyCol{0.6f, 0.7f, 1.0f};
        float sunContrib{1.0f};
        float skyContrib{0.1f};

        struct VxgiRenderTarget {
            Coel::Fbo fbo;
            Coel::Texture3D texture;
        };
        VxgiRenderTarget vxgiRenderTarget;

        glm::ivec3 vxgiRenderSize{64, 64, 64};
        glm::vec3 vxgiBoxMin{-10.0f, -10.0f, -10.0f};
        glm::vec3 vxgiBoxMax{10.0f, 10.0f, 10.0f};

        Coel::create(vxgiRenderTarget.fbo, {vxgiRenderSize.x, vxgiRenderSize.y});
        Coel::create(vxgiRenderTarget.texture, vxgiRenderSize, Coel::TextureFormat::RGBA, nullptr);
        Coel::attach(vxgiRenderTarget.fbo, {vxgiRenderTarget.texture});

        int sliceOffset = vxgiRenderSize.z / 2;
        float sliceSize = (vxgiBoxMax.z - vxgiBoxMin.z) / static_cast<float>(vxgiRenderSize.z);

        const char *const quadVertSrc = R"(
        #version 440
        layout (location = 0) in vec2 a_pos;
        layout (location = 1) in vec2 a_tex;
        out vec2 v_tex;
        void main() {
            v_tex = a_tex;
            gl_Position = vec4(a_pos, 0, 1) * vec4(0.3, 0.3, 1, 1) + vec4(0.7, 0.7, 0, 0);
        })";

        const char *const quadFragSrc = R"(
        #version 440
        in vec2 v_tex;
        out vec4 color;
        uniform sampler3D u_fboTex;
        uniform int u_sliceOffset;
        void main() {
            color = texture(u_fboTex, vec3(v_tex, float(u_sliceOffset) / 256));
            color.w = 1;
        })";

        Coel::Renderer::Quad2d quadRenderer;
        Coel::Shader quadShader;
        Coel::create(quadShader, quadVertSrc, quadFragSrc);
        auto u_fboTex = Coel::findInt(quadShader, "u_fboTex");
        auto u_sliceOffset = Coel::findInt(quadShader, "u_sliceOffset");

        while (window.isOpen) {
            // Shadow pass
            Coel::bind(shadowMapFbo);
            Coel::Renderer::enableDepthTest(true);
            Coel::Renderer::enableCulling(false);
            Coel::Renderer::clearDepth();
            glm::mat4 shadowMapProjMat = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);
            glm::mat4 shadowMapViewMat = glm::rotate(glm::mat4{1}, glm::radians(sun_elevation), {1, 0, 0});
            shadowMapViewMat = glm::rotate(shadowMapViewMat, glm::radians(sun_angle), {0, 1, 0});
            auto lightDir_init = glm::vec4{0, 0, -1, 0} * shadowMapViewMat;
            glm::vec3 lightDir = -glm::normalize(glm::vec3(lightDir_init.x, lightDir_init.y, lightDir_init.z));
            Coel::bind(shader);
            Coel::send(u_projMat, &shadowMapProjMat);
            Coel::send(u_viewMat, &shadowMapViewMat);
            Coel::Renderer::draw(model, u_modlMat, u_modlNrmMat);

            // vxgi pass
            Coel::bind(vxgiRenderTarget.fbo);
            Coel::Renderer::enableCulling(false);
            Coel::Renderer::setClearColor(0, 0, 0, 0);
            Coel::Renderer::clear();
            glm::mat4 vxgiViewMat = glm::mat4{1};
            Coel::bind(shader);
            Coel::send(u_viewMat, &vxgiViewMat);
            Coel::send(u_lightProjMat, &shadowMapProjMat);
            Coel::send(u_lightViewMat, &shadowMapViewMat);
            Coel::bind(shadowMapTexture, 1);
            Coel::send(u_shadowMap, 1);
            Coel::send(u_lightDir, &lightDir);
            Coel::send(u_sunCol, &sunCol);
            Coel::send(u_skyCol, &skyCol);
            Coel::send(u_sunContrib, &sunContrib);
            Coel::send(u_skyContrib, &skyContrib);

            for (int i = 0; i < vxgiRenderSize.z; ++i) {
                Coel::setLayer(vxgiRenderTarget.fbo, 0, i);
                glm::mat4 vxgiProjMat = glm::ortho(vxgiBoxMin.x, vxgiBoxMax.x, vxgiBoxMin.y, vxgiBoxMax.y, -sliceSize / 2 + float(i) * sliceSize, sliceSize / 2 + float(i) * sliceSize);
                Coel::send(u_projMat, &vxgiProjMat);
                Coel::Renderer::draw(model, u_modlMat, u_modlNrmMat, u_tex, u_nrm_tex);
            }

            // Main pass
            Coel::bind(Coel::Fbo{.id = 0});
            Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
            Coel::Renderer::enableCulling(true);
            Coel::Renderer::setClearColor(skyCol.r, skyCol.g, skyCol.b, 1);
            Coel::Renderer::clear();
            Coel::bind(shader);
            player.update(static_cast<float>(window.elapsed));
            camera.fov = player.fov;
            camera.resize(window.size.x, window.size.y);
            camera.set_pos(player.pos);
            camera.set_rot(player.rot.x, player.rot.y);
            projMat = camera.proj_mat;
            viewMat = camera.vrot_mat * camera.vtrn_mat;
            Coel::send(u_projMat, &projMat);
            Coel::send(u_viewMat, &viewMat);
            Coel::send(u_lightProjMat, &shadowMapProjMat);
            Coel::send(u_lightViewMat, &shadowMapViewMat);
            Coel::bind(shadowMapTexture, 1);
            Coel::send(u_shadowMap, 1);
            Coel::send(u_lightDir, &lightDir);
            Coel::send(u_sunCol, &sunCol);
            Coel::send(u_skyCol, &skyCol);
            Coel::send(u_camPos, &player.pos);
            Coel::send(u_sunContrib, &sunContrib);
            Coel::send(u_skyContrib, &skyContrib);
            Coel::Renderer::draw(model, u_modlMat, u_modlNrmMat, u_tex, u_nrm_tex);

            // Draw texture
            Coel::Renderer::enableDepthTest(false);
            Coel::Renderer::enableCulling(false);
            Coel::bind(quadShader);
            Coel::bind(vxgiRenderTarget.texture, 0);
            Coel::send(u_fboTex, 0);
            Coel::send(u_sliceOffset, sliceOffset);
            quadRenderer.draw();

            if (is_paused) {
                imgui.begin();
                ImGui::SliderFloat("sun angle", &sun_angle, 0.0f, 360.0f);
                ImGui::SliderFloat("sun elevation", &sun_elevation, 0.0f, 90.0f);
                ImGui::ColorEdit3("sun color", reinterpret_cast<float *>(&sunCol));
                ImGui::SliderFloat("sun contrib", &sunContrib, 0.0f, 50.0f);
                ImGui::ColorEdit3("sky color", reinterpret_cast<float *>(&skyCol));
                ImGui::SliderFloat("sky contrib", &skyContrib, 0.0f, 1.0f);
                ImGui::SliderInt("slice offset", &sliceOffset, 0, vxgiRenderSize.z);
                imgui.end(window);
            }

            Coel::update(window);
        }
    }

    Coel::destroy(window);
}
