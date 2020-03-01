#include <Coel.hpp>
#include <iostream>

#include "Shaders.hpp"
#include "Wire.hpp"

Coel::Window window(1400, 900, "Editor");

Coel::Renderer::Batch2d renderer;
Coel::Shader shader(vertSrc, fragSrc);

Math::Mat4 viewMat{0};

std::vector<Node> nodes;
int closestNodeIndex = -1, grabbedNodeIndex = -1;

std::vector<Wire> wires;

enum ToolMode {
    NONE,

    PLACE_NODE,
    DRAG_NODE,

    PLACE_WIRE_NODE_1,
    PLACE_WIRE_NODE_2,
};

ToolMode toolMode = NONE;

int main() {
    window.onResize = [](Coel::Window &w) {
        viewMat = Math::Mat4::ortho(0, w.size.x, w.size.y, 0, -1, 1);
        shader.sendMat4("viewMat", &viewMat);
    };

    window.onMouseMove = [](Coel::Window &w) {
        switch (toolMode) {
        case DRAG_NODE: nodes[grabbedNodeIndex].pos = {(float)w.mouse.pos.x, (float)w.mouse.pos.y}; break;
        default: break;
        }
        closestNodeIndex = getClosestNode({(float)w.mouse.pos.x, (float)w.mouse.pos.y}, nodes);
    };

    window.onKey = [](Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
            switch (w.key.code) {
            case Coel::Key::N: toolMode = PLACE_NODE; break;
            case Coel::Key::W: toolMode = PLACE_WIRE_NODE_1; break;
            default: break;
            }
            break;
        default: break;
        }
    };

    window.onMouseButton = [](Coel::Window &w) {
        switch (w.mouse.action) {
        case Coel::Action::Press:
            switch (w.mouse.button) {
            case Coel::Button::Left:
                switch (toolMode) {
                case PLACE_NODE: nodes.push_back({(float)w.mouse.pos.x, (float)w.mouse.pos.y}); break;
                case PLACE_WIRE_NODE_1:
                    if (closestNodeIndex < 0) {
                        nodes.push_back({(float)w.mouse.pos.x, (float)w.mouse.pos.y});
                        closestNodeIndex = nodes.size() - 1;
                    }
                    wires.push_back({closestNodeIndex, -1});
                    toolMode = PLACE_WIRE_NODE_2;
                    break;
                case PLACE_WIRE_NODE_2: {
                    Wire &lastWire = wires.back();
                    if (closestNodeIndex < 0) {
                        nodes.push_back({(float)w.mouse.pos.x, (float)w.mouse.pos.y});
                        closestNodeIndex = nodes.size() - 1;
                    }
                    if (closestNodeIndex != lastWire.n1) lastWire.n2 = closestNodeIndex;
                    toolMode = NONE;
                    break;
                }
                default:
                    if (closestNodeIndex > -1) {
                        toolMode = DRAG_NODE;
                        grabbedNodeIndex = closestNodeIndex;
                    }
                    break;
                }
                break;
            default: toolMode = NONE; break;
            }
            break;
        case Coel::Action::Release:
            switch (w.mouse.button) {
            case Coel::Button::Left:
                switch (toolMode) {
                case DRAG_NODE:
                    toolMode = NONE;
                    grabbedNodeIndex = -1;
                    break;
                default: break;
                }
                break;
            default: break;
            }
            break;
        default: break;
        }
    };

    window.onResize(window);
    Coel::Renderer::Command::setClearColor(0.1f, 0.1f, 0.1f, 1.f);

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();
        shader.bind();

        unsigned int index = 0;
        for (Wire &w : wires) {
            if (w.n1 > -1 && w.n2 > -1) {
                renderer.fill(0x333333ff);
                auto &p1 = nodes[w.n1].pos, &p2 = nodes[w.n2].pos;
                renderer.submitLine(p1.x, p1.y, p2.x, p2.y);
            }
        }

        for (Node &n : nodes) {
            renderer.fill(0x555555ff);
            renderer.submitEllipse(n.pos.x, n.pos.y, 5, 5);
        }
        if (grabbedNodeIndex > -1) {
            renderer.fill(0xaaaaaaff);
            const auto &pos = nodes[grabbedNodeIndex].pos;
            renderer.submitEllipse(pos.x, pos.y, 5, 5);
        }
        if (closestNodeIndex > -1) {
            renderer.fill(0x33ff3333);
            const auto &pos = nodes[closestNodeIndex].pos;
            renderer.submitEllipse(pos.x, pos.y, 10, 10);
        }

        switch (toolMode) {
        case PLACE_NODE:
            renderer.fill(0x33ff3333);
            renderer.submitEllipse(window.mouse.pos.x, window.mouse.pos.y, 4, 4);
            break;
        case PLACE_WIRE_NODE_1:
            renderer.fill(0x33ff3333);
            renderer.submitEllipse(window.mouse.pos.x, window.mouse.pos.y, 4, 4);
            break;
        case PLACE_WIRE_NODE_2: {
            renderer.fill(0x33ff3333);
            auto &p = nodes[wires.back().n1].pos;

            renderer.submitLine(p.x, p.y, window.mouse.pos.x, window.mouse.pos.y);
            renderer.submitEllipse(window.mouse.pos.x, window.mouse.pos.y, 4, 4);
            break;
        }
        default: break;
        }

        renderer.flush();
        window.update();
    }
}
