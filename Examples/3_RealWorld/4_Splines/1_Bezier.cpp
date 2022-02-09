#include <Coel.hpp>

int main() {
    Coel::Window window({800, 600}, "Title");
    Coel::create(window);
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

        vec3 solveCubic(float a, float b, float c) {
            float p = b - a*a / 3.0, p3 = p*p*p;
            float q = a * (2.0*a*a - 9.0*b) / 27.0 + c;
            float d = q*q + 4.0*p3 / 27.0;
            float offset = -a / 3.0;
            if(d >= 0.0) { 
                float z = sqrt(d);
                vec2 x = (vec2(z, -z) - q) / 2.0;
                vec2 uv = sign(x)*pow(abs(x), vec2(1.0/3.0));
                return vec3(offset + uv.x + uv.y);
            }
            float v = acos(-sqrt(-27.0 / p3) * q / 2.0) / 3.0;
            float m = cos(v), n = sin(v)*1.732050808;
            return vec3(m + m, -n - m, n - m) * sqrt(-p / 3.0) + offset;
        }

        vec3 sdBezier(vec2 A, vec2 B, vec2 C, vec2 p) {
            B = mix(B + vec2(1e-4), B, abs(sign(B * 2.0 - A - C)));
            vec2 a = B - A, b = A - B * 2.0 + C, c = a * 2.0, d = A - p;
            vec3 k = vec3(3.*dot(a,b),2.*dot(a,a)+dot(d,b),dot(d,a)) / dot(b,b);

            vec2 t = clamp(solveCubic(k.x, k.y, k.z).xy, 0.0, 1.0);
            vec2 dp1 = d + (c + b*t.x)*t.x;
            float d1 = dot(dp1, dp1);
            vec2 dp2 = d + (c + b*t.y)*t.y;
            float d2 = dot(dp2, dp2);
            
            // Find closest distance and t
            vec2 r = (d1 < d2) ? vec2(d1, t.x) : vec2(d2, t.y);
            
            // Find on which side (t=0 or t=1) is extension
            vec2 e = vec2(step(0.,-r.y),step(1.,r.y));

            // Calc. gradient
            vec2 g = 2.*b*r.y + c;
            
            // Calc. extension to t
            float et = (e.x*dot(-d,g) + e.y*dot(p-C,g))/dot(g,g);
            
            // Find closest point on curve with extension
            vec2 dp = d + (c + b*r.y)*r.y + et*g;
            
            // Sign is just cross product with gradient
            float s =  sign(g.x*dp.y - g.y*dp.x);
            
            return vec3(sqrt(r.x), s*length(dp), r.y + et);
        }

        uniform vec2 p[4];

        void main() {
            vec2 gp[3];
            gp[0] = p[1];
            gp[1] = p[2];
            gp[2] = p[3];

            vec2 cp[5];
            cp[0] = p[0];
            cp[2] = p[2];
            
            // cp[1] = (4.0 * p[1] - p[0] - p[2]) / 2;
            // cp[3] = 2.0 * p[2] - cp[1];

            cp[1] = p[1];
            cp[3] = 2.0 * p[2] - cp[1];

            cp[4] = p[3];
            // p2 = 0.4 * cp[1] - 0.2 * p[3];
            vec2 a = (5 * p[2] + p[3] - 2 * p[1]) / 4;
            vec2 b = (1 * p[2] + p[0] + 2 * p[1]) / 4;
            // vec2 c = (1 * p[2] + p[0] + 2 * p[1]) / 4;

            vec3 r1 = sdBezier(cp[0], cp[1], cp[2], v_tex);
            vec3 r2 = sdBezier(cp[2], cp[3], cp[4], v_tex);
            float r = r1.x;
            if (r1.x > r2.x) r = r2.x;
            float m = (abs(r * 400) / 400 - 0.001) * 600;
            fragCol = vec4(vec3(clamp(mix(0.0, 0.5, m), 0.f, 0.5)), 1);

            for (int i = 0; i < 4; ++i)
                if (length(p[i] - v_tex) < 0.02) fragCol = vec4(1);
            for (int i = 0; i < 5; ++i)
                if (length(cp[i] - v_tex) < 0.01) fragCol = vec4(0.6, 0.2, 0.2, 1);

            if (length(a - v_tex) < 0.01) fragCol = vec4(0.3, 0.2, 0.9, 1);
            if (length(b - v_tex) < 0.01) fragCol = vec4(0.3, 0.2, 0.9, 1);
            // if (length(c - v_tex) < 0.01) fragCol = vec4(0.3, 0.9, 0.4, 1);
        }
    )";
    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);

    glm::vec2 points[]{
        glm::vec2(0.3, 0.3),
        glm::vec2(0.2, 0.7),
        glm::vec2(0.4, 0.5),
        glm::vec2(0.8, 0.8),
    };

    auto u_p = Coel::findFloat2(shader, "p");
    u_p.count = 4;

    while (window.isOpen) {
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
        
        Coel::Renderer::clearColor();
        Coel::bind(shader);
        Coel::send(u_p, points);
        quad.draw();
        Coel::update(window);
    }
}
