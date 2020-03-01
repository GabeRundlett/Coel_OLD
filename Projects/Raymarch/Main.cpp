#include "Shaders.hpp"
#include <Coel.hpp>

#include <iostream>

Coel::Window window(1400, 800, "Raymarching");

float vdata[]{
    -1.f, -1.f, //
    -1.f, 1.f,  //
    1.f,  -1.f, //

    1.f,  -1.f, //
    -1.f, 1.f,  //
    1.f,  1.f,  //
};
Coel::Vbo vbo(vdata, sizeof(vdata), {{Coel::Element::F32, 2}});
Coel::Vao vao;

Coel::Shader shader(vertSrc, fragSrc);
Math::Vec2 viewSize;
Math::Vec3 camPos = {0, 0, 0}, camRot = {0, 0, 0};
enum CamMove {
    None = 0,
    Forward = 1 << 0,
    Backward = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3,
    Up = 1 << 4,
    Down = 1 << 5,
};
unsigned char camMove = None;
double camPosSpeed = 5.0;
Math::Vec2d camRotSens = {0.01, 0.01};

inline void sendView() {
    shader.sendFloat3("viewPos", &camPos);
    shader.sendFloat3("viewRot", &camRot);
}

int main() {
    window.onResize = [](Coel::Window &w) {
        viewSize = {float(w.size.x), float(w.size.y)};
        shader.sendFloat2("viewSize", &viewSize);
    };
    window.onResize(window);
    window.onKey = [](Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
            switch (w.key.code) {
            case Coel::Key::W: camMove |= Forward; break;
            case Coel::Key::S: camMove |= Backward; break;
            case Coel::Key::A: camMove |= Left; break;
            case Coel::Key::D: camMove |= Right; break;
            case Coel::Key::Space: camMove |= Up; break;
            case Coel::Key::LeftShift: camMove |= Down; break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (w.key.code) {
            case Coel::Key::W: camMove &= ~Forward; break;
            case Coel::Key::S: camMove &= ~Backward; break;
            case Coel::Key::A: camMove &= ~Left; break;
            case Coel::Key::D: camMove &= ~Right; break;
            case Coel::Key::Space: camMove &= ~Up; break;
            case Coel::Key::LeftShift: camMove &= ~Down; break;
            default: break;
            }
            break;
        default: break;
        }
    };
    window.onMouseMove = [](Coel::Window &w) {
        Math::Vec2d center{double(w.size.x / 2), double(w.size.y / 2)};
        auto diffMouse = (window.mouse.pos - center) * camRotSens;
        camRot.x -= diffMouse.y, camRot.z -= diffMouse.x;
        window.cursorTo(center);
    };

    vao.add(vbo);

    auto time = window.getTime(), prevTime = time, elapsed = 0.0;

    while (window.isOpen()) {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;

        if (camMove) {
            auto c = std::cos(camRot.z), s = std::sin(camRot.z);
            if (camMove & Forward) {
                camPos.x -= camPosSpeed * elapsed * s;
                camPos.y += camPosSpeed * elapsed * c;
            }
            if (camMove & Backward) {
                camPos.x += camPosSpeed * elapsed * s;
                camPos.y -= camPosSpeed * elapsed * c;
            }
            if (camMove & Left) {
                camPos.x -= camPosSpeed * elapsed * c;
                camPos.y -= camPosSpeed * elapsed * s;
            }
            if (camMove & Right) {
                camPos.x += camPosSpeed * elapsed * c;
                camPos.y += camPosSpeed * elapsed * s;
            }
            if (camMove & Up) camPos.z += camPosSpeed * elapsed;
            if (camMove & Down) camPos.z -= camPosSpeed * elapsed;
        }

        sendView();
        vao.draw(6);

        window.update();
    }
}
