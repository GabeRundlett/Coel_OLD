#pragma once

#include <memory>
#include <functional>
#include <Coel/Input.hpp>
#include <Coel/Renderer/Buffer.hpp>

struct GLFWwindow;

namespace Coel {
    enum CursorMode {
        Normal,
        Hidden,
        Disabled,
    };

    struct Window final {
        glm::ivec2 pos{0, 0}, size{800, 600};
        const char *const title{"Coel Window"};
        Fbo fbo{.id = 0, .size = size, .drawBufferN = 0, .drawBuffers = {}};
        KeyInfo key{};
        MouseInfo mouse{};
        GLFWwindow *glfwHandle{};
        double time{}, prevTime{}, elapsed{};
        std::function<void(Window &)> onResize{}, onFboResize{};
        std::function<void(Window &)> onMouseButton{}, onMouseMove{}, onMouseScroll{};
        std::function<void(Window &)> onKey{}, onChar{};
        bool isOpen{true};
        Window(const glm::ivec2 size, const char *const title) : size(size), title{title}, fbo{.id = 0, .size = size, .drawBufferN = 0, .drawBuffers = {}} {}
        Window(const char *const title) : title{title} {}
        Window() = default;
    };

    int create(Window &window);
    void destroy(Window &window);
    void update(Window &window);
    double getTime();
    void cursorTo(Window &window, const glm::dvec2 &pos);
    void cursorMode(const Window &window, const CursorMode mode);
    void resize(Window &window);
    void bind(const Window &window);
} // namespace Coel
