#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Simple Textured Quad Example");

    float vertex_data[]{
        -0.5, -0.5, 0, 0, //
        -0.5, 0.5,  0, 1, //
        0.5,  -0.5, 1, 0, //
        0.5,  0.5,  1, 1  //
    };
    // New Attrib {F32, 2}
    Coel::Vbo vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}, {Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo(index_data, sizeof(index_data));

    // --------------------------------------------------------------
    // We'll turn our second attribute and the varying variable
    // from the previous example (Shaders/Varying) to store and
    // pass the texture coordinates that we'll use to sample the
    // texture in the fragment shader.
    // --------------------------------------------------------------
    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    // --------------------------------------------------------------
    // Now we add a new uniform `sampler2D u_tex`, which is
    // actually just an int that we'll send in the application
    // loop. We'll then sample the sampler2D in main by
    // calling `texture();`
    // --------------------------------------------------------------
    const char *const fragSrc = R"(
    #version 450 core
    in vec2 v_tex;
    out vec4 frag_color;

    uniform sampler2D u_tex;
    
    void main() {
        frag_color = texture(u_tex, v_tex);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    // --------------------------------------------------------------
    // So lets create that uniform we were talking about:
    auto u_tex = shader.findInt("u_tex");

    // And we obviously have to create a texture, so lets do so.
    // We'll load one from a file. We'll specify the file relative
    // to the invocation directory, which is assumed to be the root
    // directory of this repository.
    Coel::Texture texture("Assets/face.png");
    // --------------------------------------------------------------

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        shader.bind();

        // --------------------------------------------------------------
        // Now, when we go to draw our quad, we'll bind our texture
        // to the 0th texture slot, and tell the shader that our
        // texture is at that slot.
        shader.send(u_tex, 0);
        texture.bind(0);
        // --------------------------------------------------------------

        vao.drawIndexed(6);

        window.update();
    }
}
