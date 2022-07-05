#include <Coel.hpp>
#include <iostream>

#include "0_Player.hpp"
#include "../../1_GettingStarted/6_PostProcess/0_Common/Ssao.hpp"
#include "../../1_GettingStarted/6_PostProcess/0_Common/Shaders/Deferred.hpp"

int main() {
    Coel::Window window("Simple Shadow Mapping Example");
    Coel::create(window);

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        // const char *const vertSrc = R"(
        // #version 450 core
        // layout (location = 0) in vec3 a_pos;
        // layout (location = 1) in vec3 a_nrm;
        // layout (location = 2) in vec2 a_tex;
        // out vec3 v_pos;
        // out vec3 v_lightSpacePos;
        // out vec3 v_nrm;
        // out vec2 v_tex;
        // uniform mat4 u_projMat;
        // uniform mat4 u_viewMat;
        // uniform mat4 u_lightProjMat;
        // uniform mat4 u_lightViewMat;
        // uniform mat4 u_modlMat;
        // uniform mat4 u_modlNrmMat;
        // void main() {
        //     vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        //     vec4 worldNrm = u_modlNrmMat * vec4(a_nrm, 1);
        //     v_pos = worldPos.xyz;
        //     vec4 lightSpacePos = u_lightProjMat * u_lightViewMat * worldPos;
        //     v_lightSpacePos = lightSpacePos.xyz / lightSpacePos.w / 2 + 0.5;
        //     v_nrm = worldNrm.xyz, v_tex = vec2(a_tex.x, a_tex.y);
        //     gl_Position = u_projMat * u_viewMat * worldPos;
        // })";

        // const char *const fragSrc = R"(
        // #version 450 core
        // in vec3 v_pos;
        // in vec3 v_lightSpacePos;
        // in vec3 v_nrm;
        // in vec2 v_tex;
        // out vec4 frag_color;
        // uniform sampler2D u_tex;
        // uniform sampler2D u_nrm_tex;
        // uniform sampler2D u_shadowMap;
        // uniform vec3 u_lightDir;
        // uniform vec3 u_sunCol;
        // uniform vec3 u_skyCol;
        // uniform vec3 u_camPos;
        // uniform float u_sunContrib;
        // uniform float u_skyContrib;
        // float shadowCalc(in float dotLightNrm) {
        //     vec3 pos = v_lightSpacePos;
        //     if (pos.z > 1) pos.z = 1;
        //     float factor = 0.f;
        //     float bias = max(0.005 * (1 - dotLightNrm), 0.0005);
        //     vec2 texelSize = 1.f / textureSize(u_shadowMap, 0);
        //     for (int y = -2; y <= 2; ++y) {
        //         for (int x = -2; x <= 2; ++x) {
        //             float depth = texture(u_shadowMap, pos.xy + vec2(float(x), float(y)) * texelSize).r;
        //             if (depth + bias < pos.z)
        //                 factor += 1.0f / (5 * 5);
        //         }
        //     }
        //     return 1 - factor;
        // }
        // vec3 filmicToneMapping(vec3 color) {
        //     color = max(vec3(0.), color - vec3(0.004));
        //     color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
        //     return color;
        // }
        // mat3 get_tbn(vec3 n, vec3 p, vec2 uv) {
        //     vec3 dp1 = dFdx(p);
        //     vec3 dp2 = dFdy(p);
        //     vec2 duv1 = dFdx(uv);
        //     vec2 duv2 = dFdy(uv);
        //     vec3 dp2perp = cross(dp2, n);
        //     vec3 dp1perp = cross(n, dp1);
        //     vec3 t = dp2perp * duv1.x + dp1perp * duv2.x;
        //     vec3 b = dp2perp * duv1.y + dp1perp * duv2.y;
        //     float invmax = inversesqrt(max(dot(t, t), dot(b, b)));
        //     return mat3(t * invmax, b * invmax, n);
        // }
        // void main() {
        //     vec3 lightDir = normalize(u_lightDir);
        //     vec3 v_col = texture(u_tex, v_tex).rgb;
        //     // v_col = pow(max(v_col, 0), vec3(2.2));
        //     vec3 lightCol = vec3(1);
        //     vec3 nrm = normalize(v_nrm);
        //     vec3 nrm_tex = normalize(texture(u_nrm_tex, v_tex).xyz * 2.0f - 1.0f);
        //     nrm_tex = normalize(nrm_tex + vec3(0, 0, 1));
        //     nrm = get_tbn(nrm, v_pos, v_tex) * nrm_tex;
        //     float dotLightNrm = dot(lightDir, nrm);
        //     float diff = 1.0f; //max(dotLightNrm, 0);
        //     float shadow = shadowCalc(clamp(dotLightNrm, 0, 1));
        //     diff *= shadow;
        //     diff = max(diff, 0.0) * 1;
        //     vec3 posToCam = normalize(v_pos + u_camPos);
        //     vec3 reflPosToCam = reflect(-posToCam, nrm);
        //     vec3 posToLight = lightDir;
        //     vec3 spec = u_sunContrib * u_sunCol * shadow * max(pow(dot(-reflPosToCam, posToLight), 400), 0);
        //     vec3 color = (u_sunCol * diff * u_sunContrib + u_skyCol * u_skyContrib) * v_col + spec;
        //     color = filmicToneMapping(color);
        //     frag_color = vec4(color, 1);
        // })";

        const char *const lightVertSrc = R"(
        #version 450 core
        layout (location = 0) in vec3 a_pos;
        layout (location = 1) in vec3 a_nrm;
        layout (location = 2) in vec2 a_tex;
        uniform mat4 u_projMat;
        uniform mat4 u_viewMat;
        uniform mat4 u_modlMat;
        void main() {
            gl_Position = u_projMat * u_viewMat * u_modlMat * vec4(a_pos, 1);
        })";

        const char *const lightFragSrc = R"(
        #version 450 core
        void main() {
        })";

        // Coel::Shader shader;
        // Coel::create(shader, vertSrc, fragSrc);

        Coel::Shader lightShader;
        Coel::create(lightShader, lightVertSrc, lightFragSrc);

        // auto u_projMat = Coel::findMat4(shader, "u_projMat");
        // auto u_viewMat = Coel::findMat4(shader, "u_viewMat");
        // auto u_lightProjMat = Coel::findMat4(shader, "u_lightProjMat");
        // auto u_lightViewMat = Coel::findMat4(shader, "u_lightViewMat");
        // auto u_modlMat = Coel::findMat4(shader, "u_modlMat");
        // auto u_modlNrmMat = Coel::findMat4(shader, "u_modlNrmMat");

        // auto u_shadowMap = Coel::findInt(shader, "u_shadowMap");
        // auto u_lightDir = Coel::findFloat3(shader, "u_lightDir");
        // auto u_sunCol = Coel::findFloat3(shader, "u_sunCol");
        // auto u_skyCol = Coel::findFloat3(shader, "u_skyCol");
        // auto u_camPos = Coel::findFloat3(shader, "u_camPos");
        // auto u_sunContrib = Coel::findFloat(shader, "u_sunContrib");
        // auto u_skyContrib = Coel::findFloat(shader, "u_skyContrib");

        auto u_light_projMat = Coel::findMat4(lightShader, "u_projMat");
        auto u_light_viewMat = Coel::findMat4(lightShader, "u_viewMat");
        auto u_light_modlMat = Coel::findMat4(lightShader, "u_modlMat");

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

        // auto u_tex = Coel::findInt(shader, "u_tex");
        // auto u_nrm_tex = Coel::findInt(shader, "u_nrm_tex");
        std::vector<Coel::Model> objects;
        objects.resize(2);
        Coel::open(objects[0], "C:/users/gabe/Downloads/gonza/export/y-up/gonza.gltf");
        Coel::open(objects[1], "Assets/Objects/frog/scene.gltf");

        for (auto &o : objects[1].objects)
            o.modlMat *= glm::rotate(glm::translate(glm::mat4(1), {0, 0.4f, 0}), 0.25f, {1, 0, 0});

        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);

        Coel::Fbo shadowMapFbo;
        Coel::create(shadowMapFbo, {512 * 8, 512 * 8});
        Coel::Texture shadowMapTexture;
        Coel::create(shadowMapTexture, shadowMapFbo.size, Coel::TextureFormat::Depth, nullptr);
        Coel::setMagFilter(shadowMapTexture, Coel::TextureFilter::Nearest);
        Coel::attach(shadowMapFbo, {shadowMapTexture});

        const char *const quadVertSrc = R"(
        #version 450
        layout (location = 0) in vec2 a_pos;
        layout (location = 1) in vec2 a_tex;
        out vec2 v_tex;
        void main() {
            v_tex = a_tex;
            gl_Position = vec4(a_pos, 0, 1);
        })";

        const char *const quadFragSrc = R"(
        #version 450
        in vec2 v_tex;
        out vec4 color;
        uniform sampler2D u_posTex;
        uniform sampler2D u_nrmTex;
        uniform sampler2D u_colTex;
        uniform sampler2D u_ssaoTex;
        
        uniform mat4 u_projMat;
        uniform mat4 u_viewMat;
        uniform mat4 u_lightProjMat;
        uniform mat4 u_lightViewMat;
        uniform mat4 u_modlMat;
        uniform mat4 u_modlNrmMat;

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
            float bias = max(0.005 * (1 - dotLightNrm), 0.0005);
            vec2 texelSize = 1.f / textureSize(u_shadowMap, 0);

            for (int y = -2; y <= 2; ++y) {
                for (int x = -2; x <= 2; ++x) {
                    float depth = texture(u_shadowMap, pos.xy + vec2(float(x), float(y)) * texelSize).r;
                    if (depth + bias < pos.z)
                        factor += 1.0f / (5 * 5);
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
            vec3 pos = texture(u_posTex, v_tex).xyz;
            vec3 nrm = normalize(texture(u_nrmTex, v_tex).xyz);
            // vec3 col = texture(u_colTex, v_tex).rgb;
            vec3 col = vec3(1);
            float ao = texture(u_ssaoTex, v_tex).r;
            
            // vec3 lightDir = normalize(u_lightDir);
            // float dotLightNrm = dot(lightDir, nrm);
            // float diff = max(dotLightNrm, 0);
            // float shadow = shadowCalc(clamp(dotLightNrm, 0, 1));
            // diff *= shadow;
            // diff = max(diff, 0.0) * 1;

            color = vec4(col, 1);
            // color = vec4(vec3(diff), 1);
            color.rgb *= vec3(ao);
        })";

        Coel::Shader deferredShader;
        Coel::create(deferredShader, Deferred::vertSrc, Deferred::fragSrc);
        auto u_deferred_projMat = Coel::findMat4(deferredShader, "u_projMat");
        auto u_deferred_viewMat = Coel::findMat4(deferredShader, "u_viewMat");
        auto u_deferred_modlMat = Coel::findMat4(deferredShader, "u_modlMat");
        auto u_deferred_modlNrmMat = Coel::findMat4(deferredShader, "u_modlNrmMat");
        auto u_deferred_tex = Coel::findInt(deferredShader, "u_tex");
        auto u_deferred_nrm_tex = Coel::findInt(deferredShader, "u_nrm_tex");

        Coel::Shader quadShader;
        Coel::create(quadShader, quadVertSrc, quadFragSrc);
        auto u_quad_posTex = Coel::findInt(quadShader, "u_posTex");
        auto u_quad_nrmTex = Coel::findInt(quadShader, "u_nrmTex");
        auto u_quad_colTex = Coel::findInt(quadShader, "u_colTex");
        auto u_quad_ssaoTex = Coel::findInt(quadShader, "u_ssaoTex");

        auto u_quad_projMat = Coel::findMat4(quadShader, "u_projMat");
        auto u_quad_lightProjMat = Coel::findMat4(quadShader, "u_lightProjMat");
        auto u_quad_lightViewMat = Coel::findMat4(quadShader, "u_lightViewMat");

        auto u_quad_shadowMap = Coel::findInt(quadShader, "u_shadowMap");
        auto u_quad_lightDir = Coel::findFloat3(quadShader, "u_lightDir");
        auto u_quad_sunCol = Coel::findFloat3(quadShader, "u_sunCol");
        auto u_quad_skyCol = Coel::findFloat3(quadShader, "u_skyCol");
        auto u_quad_camPos = Coel::findFloat3(quadShader, "u_camPos");
        auto u_quad_sunContrib = Coel::findFloat(quadShader, "u_sunContrib");
        auto u_quad_skyContrib = Coel::findFloat(quadShader, "u_skyContrib");

        Ssao::init(window.size);
        Coel::Renderer::Quad2d quadRenderer;

        float sun_angle = 45.0f, sun_elevation = 45.0f;
        glm::vec3 sunCol{1.0f, 0.8f, 0.6f};
        glm::vec3 skyCol{0.6f, 0.7f, 1.0f};
        float sunContrib{1.0f};
        float skyContrib{0.1f};

        while (window.isOpen) {
            // Update state
            player.update(static_cast<float>(window.elapsed));
            camera.fov = player.fov;
            camera.resize(window.size.x, window.size.y);
            camera.set_pos(player.pos);
            camera.set_rot(player.rot.x, player.rot.y);
            projMat = camera.proj_mat;
            viewMat = camera.vrot_mat * camera.vtrn_mat;

            // Shadow pass
            Coel::bind(shadowMapFbo);
            Coel::Renderer::enableDepthTest(true);
            Coel::Renderer::enableCulling(false);
            Coel::Renderer::clearDepth();
            glm::mat4 shadowMapProjMat = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 20.f);
            // glm::mat4 shadowMapProjMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.01f, 10.0f);
            glm::mat4 shadowMapViewMat = glm::translate(glm::rotate(glm::mat4{1}, glm::radians(sun_elevation), {1, 0, 0}), {0, 0, 0});
            shadowMapViewMat = glm::rotate(shadowMapViewMat, glm::radians(sun_angle), {0, 1, 0});
            auto lightDir_init = glm::vec4{0, 0, -1, 0} * shadowMapViewMat;
            glm::vec3 lightDir = -glm::normalize(glm::vec3(lightDir_init.x, lightDir_init.y, lightDir_init.z));
            Coel::bind(lightShader);
            Coel::send(u_light_projMat, &shadowMapProjMat);
            Coel::send(u_light_viewMat, &shadowMapViewMat);
            for (auto &o : objects)
                Coel::Renderer::draw(o, u_light_modlMat);

            // GBuffer pass
            Coel::bind(Ssao::gbufferFbo);
            Coel::bind(deferredShader);
            Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
            Coel::Renderer::enableDepthTest(true);
            Coel::Renderer::enableCulling(true);
            Coel::Renderer::setClearColor(skyCol.r, skyCol.g, skyCol.b, 1);
            // Coel::Renderer::setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            Coel::Renderer::setClearDepth(1.0f);
            Coel::Renderer::clear();
            Coel::send(u_deferred_projMat, &projMat);
            Coel::send(u_deferred_viewMat, &viewMat);
            for (auto &o : objects)
                Coel::Renderer::draw(o, u_deferred_modlMat, u_deferred_modlNrmMat, u_deferred_tex, u_deferred_nrm_tex);
            Ssao::draw(window.size, quadRenderer, projMat, viewMat);

            Coel::bind(window.fbo);
            Coel::Renderer::enableDepthTest(false);
            Coel::Renderer::enableCulling(false);
            Coel::bind(quadShader);

            Coel::send(u_quad_projMat, &projMat);
            Coel::send(u_quad_lightProjMat, &shadowMapProjMat);
            Coel::send(u_quad_lightViewMat, &shadowMapViewMat);
            Coel::bind(shadowMapTexture, 6);
            Coel::send(u_quad_shadowMap, 6);
            Coel::send(u_quad_lightDir, &lightDir);
            Coel::send(u_quad_sunCol, &sunCol);
            Coel::send(u_quad_skyCol, &skyCol);
            Coel::send(u_quad_camPos, &player.pos);
            Coel::send(u_quad_sunContrib, &sunContrib);
            Coel::send(u_quad_skyContrib, &skyContrib);

            Coel::send(u_quad_posTex, 0);
            Coel::bind(Ssao::gbufferPosTex, 0);
            Coel::send(u_quad_nrmTex, 1);
            Coel::bind(Ssao::gbufferNrmTex, 1);
            Coel::send(u_quad_colTex, 2);
            Coel::bind(Ssao::gbufferColTex, 2);
            Coel::send(u_quad_ssaoTex, 3);
            Coel::bind(Ssao::blurTex, 3);
            quadRenderer.draw();

            // Main pass
            // Coel::bind(window.fbo);
            // Coel::Renderer::enableCulling(true);
            // Coel::Renderer::resizeViewport(0, 0, window.size.x, window.size.y);
            // Coel::Renderer::setClearColor(skyCol.r, skyCol.g, skyCol.b, 1);
            // Coel::Renderer::clear();
            // Coel::bind(shader);
            // Coel::send(u_projMat, &projMat);
            // Coel::send(u_viewMat, &viewMat);
            // Coel::send(u_lightProjMat, &shadowMapProjMat);
            // Coel::send(u_lightViewMat, &shadowMapViewMat);
            // Coel::bind(shadowMapTexture, 6);
            // Coel::send(u_shadowMap, 6);
            // Coel::send(u_lightDir, &lightDir);
            // Coel::send(u_sunCol, &sunCol);
            // Coel::send(u_skyCol, &skyCol);
            // Coel::send(u_camPos, &player.pos);
            // Coel::send(u_sunContrib, &sunContrib);
            // Coel::send(u_skyContrib, &skyContrib);
            // for (auto &o : objects)
            //     Coel::Renderer::draw(o, u_modlMat, u_modlNrmMat, u_tex, u_nrm_tex);

            if (is_paused) {
                imgui.begin();
                ImGui::DragFloat("sun angle", &sun_angle, 0.01f * 360, 0.0f, 360.0f);
                ImGui::DragFloat("sun elevation", &sun_elevation, 0.01f * 90, 0.0f, 90.0f);
                ImGui::ColorEdit3("sun color", reinterpret_cast<float *>(&sunCol));
                ImGui::DragFloat("sun contrib", &sunContrib, 0.01f * 50, 0.0f, 50.0f);
                ImGui::ColorEdit3("sky color", reinterpret_cast<float *>(&skyCol));
                ImGui::DragFloat("sky contrib", &skyContrib, 0.01f, 0.0f, 1.0f);
                imgui.end(window);
            }

            Coel::update(window);
        }
    }

    Coel::destroy(window);
}
