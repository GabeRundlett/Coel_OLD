#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Title");
    Coel::Renderer::Quad2d quad;
    const char *const vertSrc = R"(
        #version 450
        
        layout (location = 0) in vec2 a_pos;
        layout (location = 1) in vec2 a_tex;
        out vec2 v_tex;

        void main() {
            v_tex = a_tex;
            gl_Position = vec4(a_pos, 0, 1);
        }
    )";
    const char *const fragSrc = R"(
        #version 450

        in vec2 v_tex;
        out vec4 fragCol;

        uniform vec2 p[4];
        uniform float time;

        void main() {
            fragCol = vec4(0, 0, 0, 1);
            float t = sin(time) / 2 + 0.5, t2 = t * t, t3 = t2 * t;

            float q[4];
            q[0] = -t3 + 2.f * t2 - t;
            q[1] = 3.f * t3 - 5.f * t2 + 2.f;
            q[2] = -3.f * t3 + 4.f * t2 + t;
            q[3] = t3 - t2;

            vec2 pos = vec2(0);
            for (int i = 0; i < 4; ++i)
                pos += p[i] * q[i] * 0.5f;
            
            for (int i = 0; i < 4; ++i)
                if (length(p[i] - v_tex) < 0.01) fragCol = vec4(1);
            if (length(pos - v_tex) < 0.01) fragCol = vec4(1, 0, 1, 1);
        }
    )";
    Coel::Shader shader(vertSrc, fragSrc);

    glm::vec2 points[]{
        glm::vec2(0.48, 0.3),
        glm::vec2(0.2, 0.7),
        glm::vec2(0.8, 0.7),
        glm::vec2(0.52, 0.3),
    };

    auto u_p = shader.findFloat2("p");
    auto u_time = shader.findFloat("time");
    u_p.count = 4;

    while (window.isOpen()) {
        auto time = window.getTime();
        if (window.key.code == '1') {
            points[0] = glm::vec2(window.mouse.pos) / glm::vec2(window.size);
            points[0].y = 1 - points[0].y;
        }
        if (window.key.code == '2') {
            points[1] = glm::vec2(window.mouse.pos) / glm::vec2(window.size);
            points[1].y = 1 - points[1].y;
        }
        if (window.key.code == '3') {
            points[2] = glm::vec2(window.mouse.pos) / glm::vec2(window.size);
            points[2].y = 1 - points[2].y;
        }
        if (window.key.code == '4') {
            points[3] = glm::vec2(window.mouse.pos) / glm::vec2(window.size);
            points[3].y = 1 - points[3].y;
        }
        shader.bind();
        shader.send(u_p, points);
        shader.send(u_time, time);
        quad.draw();
        window.update();
    }
}
