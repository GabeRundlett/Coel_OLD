static constexpr const char *const vertSrc = R"(
#version 450
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex;
uniform mat4 u_proj;
uniform mat4 u_view;
out vec2 v_tex;

void main() {
    v_tex = a_tex;
    gl_Position = u_proj * u_view * vec4(a_pos, 1);
})";
static constexpr const char *const fragSrc = R"(
#version 450
in vec2 v_tex;
out vec4 frag_color;

void main() {
    frag_color = vec4(1, 0, 0, 1);
})";
