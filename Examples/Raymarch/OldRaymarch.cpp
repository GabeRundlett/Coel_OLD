#include <Coel.hpp>

enum UniformFlags {
    SEND_RESIZE = 1, //
};
enum CameraFlags {
    FORWARD = 1, //
    BACKWARD = 2,
    LEFT = 4,
    RIGHT = 8,
    ROTATE = 16,
    ON_GROUND = 32
};
enum MouseFlags {
    DRAG = 1, //
    HIDE = 2,
};
unsigned char sendUniformFlags = 0, camFlags = 0, mouseFlags = 0;
Math::Vec2 size = {800, 600};
Math::Vec3 camPos{0, 2, 0}, camVel{0, 0, 0};
Math::Vec2 camRot{0, 0}, camInitialRot{0, 0};
Math::Vec2d mousePos{0, 0}, mouseInitialPos{0, 0};
constexpr float speed = 10, jumpPower = 12, gravity = 40, CAM_RAD = 0.5;

void windowResize(const Coel::WindowResize &w) {
    size = {(float)w.size.x, (float)w.size.y};
    sendUniformFlags |= SEND_RESIZE;
}

void keyPress(const Coel::KeyPress &e) {
    if (e.key == Coel::W)
        camFlags |= FORWARD;
    else if (e.key == Coel::S)
        camFlags |= BACKWARD;
    else if (e.key == Coel::A)
        camFlags |= LEFT;
    else if (e.key == Coel::D)
        camFlags |= RIGHT;
    else if (e.key == Coel::Space && camFlags & ON_GROUND)
        camVel.y += jumpPower, camFlags &= ~ON_GROUND;
    else if (e.key == Coel::Escape) {
        if (mouseFlags & DRAG) mouseFlags |= HIDE;
        mouseFlags &= ~DRAG;
    }
}

void keyRelease(const Coel::KeyRelease &e) {
    if (e.key == Coel::W)
        camFlags &= ~FORWARD;
    else if (e.key == Coel::S)
        camFlags &= ~BACKWARD;
    else if (e.key == Coel::A)
        camFlags &= ~LEFT;
    else if (e.key == Coel::D)
        camFlags &= ~RIGHT;
}

void mousePress(const Coel::MousePress &e) {
    if (e.button == 0) {
        mouseInitialPos = mousePos;
        camInitialRot = camRot;
        mouseFlags |= HIDE;
        mouseFlags |= DRAG;
    } else if (e.button == 1) {
        if (mouseFlags & DRAG) mouseFlags |= HIDE;
        mouseFlags &= ~DRAG;
    }
}

void mouseMove(const Coel::MouseMove &e) {
    mousePos = e.offset;
    if (mouseFlags & DRAG) {
        Math::Vec2d tSize = {(double)size.x, (double)size.y};
        auto offset = (mousePos - tSize / 2) / tSize.y * 1;
        offset.x *= 2;
        camRot += {float(offset.x), float(offset.y)};
        if (camRot.y > 1)
            camRot.y = 1;
        else if (camRot.y < -1)
            camRot.y = -1;

        camFlags |= ROTATE;
    }
}

int main() {
    auto window = Coel::createWindow((int)size.x, (int)size.y, "Simple Shader Example");
    window->onWindowResize(windowResize);
    window->onKeyPress(keyPress);
    window->onKeyRelease(keyRelease);
    window->onMousePress(mousePress);
    window->onMouseMove(mouseMove);

    const char *const vertSrc = R"(
    #version 440

    layout (location = 0) in vec2 pos;

    void main() {
	    gl_Position = vec4(pos, 0, 1);
    })",
                      *const fragSrc = R"(
    #version 440

    uniform float time;
    uniform vec2 size;
    uniform vec3 camPos;
    uniform vec2 camRot;
    uniform mat4 viewMat;

    out vec4 color;
    #define ITER_COUNT 1000
    #define MIN_DIST_INC 0.0001
    #define MAX_DIST 1000

    #define NONE   0
    #define LIGHT  1
    #define PLANE  2
    #define SPHERE 3

    struct SceneClass {
        int type, index;
    };
    struct SceneInfo {
        float dist;
        SceneClass obj;
    };

    struct Light {
        vec3 pos;
        float rad, pow;
    };
    struct Plane {
        vec3 pos, nrm;
    };
    struct Sphere {
        vec3 pos;
        float rad;
    };

    const int lightCount = 2;
    Light lights[lightCount];
    const int planeCount = 6;
    Plane planes[planeCount];
    const int sphereCount = 3;
    Sphere spheres[sphereCount];

    float sphereDE(in vec3 rayPos, in vec3 sPos, in float r) {
        return length(sPos - rayPos) - r;
    }

    SceneInfo sceneInfo(vec3 rayPos) {
        SceneInfo info;
        info.dist = MAX_DIST;
        info.obj.type = NONE;
        info.obj.index = 0;

        for (int i = 0; i < lightCount; ++i) {
            Light l = lights[i];
            float d = sphereDE(rayPos, l.pos, l.rad);
            if (d < info.dist) {
                info.dist = d;
                info.obj.type = LIGHT;
                info.obj.index = i;
            }
        }
        for (int i = 0; i < planeCount; ++i) {
            Plane p = planes[i];
            float d = length(abs(rayPos - p.pos) * p.nrm);
            if (d < info.dist) {
                info.dist = d;
                info.obj.type = PLANE;
                info.obj.index = i;
            }
        }
        for (int i = 0; i < sphereCount; ++i) {
            Sphere s = spheres[i];
            float d = sphereDE(rayPos, s.pos, s.rad);
            if (d < info.dist) {
                info.dist = d;
                info.obj.type = SPHERE;
                info.obj.index = i;
            }
        }
        return info;
    }

    bool march(inout vec3 rayPos, inout float totalDist, inout float minDist, inout SceneClass obj, in vec3 unitRay) {
        SceneInfo info = sceneInfo(rayPos);
    
        rayPos += unitRay * info.dist;
        totalDist += info.dist;
    
        if (info.dist * info.dist < MIN_DIST_INC * MIN_DIST_INC) {
            obj = info.obj;
            return true;
        }
        else if (totalDist > MAX_DIST) {
            obj.type = NONE;
            obj.index = 0;
            return true;
        }

        if (info.dist < minDist) minDist = info.dist;
        return false;
    }

    float lightShade(in vec3 rPos, in vec3 normal, in int lightIndex) {
        Light l = lights[lightIndex];

        vec3 unitRay = l.pos - rPos;
        float distToLight = length(unitRay);
        float intensity = l.pow / distToLight / distToLight;
        unitRay /= distToLight;

        float totalDist = 0, minDist = MAX_DIST;
        SceneClass obj;
    
        if (intensity > 0.001)
            for (int iter = 0; iter < ITER_COUNT; ++iter)
                if (march(rPos, totalDist, minDist, obj, unitRay)) break;
    
        if (obj.type == LIGHT) 
            if (obj.index == lightIndex) 
                return intensity * dot(normal, unitRay);
        return 0; 
    }

    float sceneShade(inout vec3 rayPos, in vec3 normal) {
        rayPos += normal * MIN_DIST_INC * 1.01;
        float shade = 0;
        for (int i = 0; i < lightCount; ++i)
            shade += lightShade(rayPos, normal, i);
        return shade;
    }

    void main() {
        float sin_time = time, cos_time = time;

        lights[0].pos = vec3(1, 2.5, 1);
        lights[0].rad = 0.1;
        lights[0].pow = 2;

        lights[1].pos = vec3(0, 3, 0);
        lights[1].rad = 0.1;
        lights[1].pow = 2;

        planes[0].pos = vec3(-4, 0, 0);
        planes[0].nrm = vec3( 1, 0, 0);

        planes[1].pos = vec3( 4, 0, 0);
        planes[1].nrm = vec3(-1, 0, 0);

        planes[2].pos = vec3(0, 0, -4);
        planes[2].nrm = vec3(0, 0,  1);

        planes[3].pos = vec3(0, 0,  4);
        planes[3].nrm = vec3(0, 0, -1);

        planes[4].pos = vec3(0, 0, 0);
        planes[4].nrm = vec3(0, 1, 0);

        planes[5].pos = vec3(0,  5, 0);
        planes[5].nrm = vec3(0, -1, 0);

        spheres[0].pos = vec3(0, 0.75, 2);
        spheres[0].rad = 0.75;

        spheres[1].pos = vec3(1, 0.5, 1);
        spheres[1].rad = 0.5;
    
        spheres[2].pos = vec3(-0.75, 0.25, 0.75);
        spheres[2].rad = 0.25;

        const float aspectRatio = size.x / size.y;

	    vec3 rayPos = camPos;
        vec2 tex = gl_FragCoord.xy / size;
        vec2 viewPos = vec2((tex.x - 0.5) * aspectRatio, tex.y - 0.5);
        vec3 viewAngle = normalize(vec3(sin(viewPos.x), sin(viewPos.y), cos(viewPos.x)));

        vec4 viewRay = viewMat * vec4(viewAngle, 1);
        vec3 unitRay = normalize(viewRay.xyz);

        float totalDist = 0, minDist = MAX_DIST;
        SceneClass obj;
	
        for (int iter = 0; iter < ITER_COUNT; ++iter) 
            if (march(rayPos, totalDist, minDist, obj, unitRay)) break;

        switch (obj.type) {
            case NONE:
    	        color = vec4(1, 0, 1, 1);
                break;
            case LIGHT:
                color = vec4(1, 1, 1, 1);
                break;
            case PLANE:
                Plane p = planes[obj.index];
                rayPos += p.nrm * MIN_DIST_INC;
                color = vec4(1, 1, 1, 1) * sceneShade(rayPos, p.nrm);
                break;
            case SPHERE:
                Sphere s = spheres[obj.index];
                vec3 normal = normalize(rayPos - s.pos);
                rayPos = s.pos + normal * s.rad;
                color = vec4(1, 0, 0, 1) * sceneShade(rayPos, normal);
                break;
        }
        //color = vec4(viewPos * 2, 0, 1);
    })";

    struct Vertex {
        Math::Vec2 pos;
    };
    Vertex vertices[] = {
        {-1, -1},
        {1, -1},
        {-1, 1},
        {1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);
    auto shader = Coel::createShader(vertSrc, fragSrc);

    {
        shader->sendFloat2("size", &size);
        shader->sendFloat3("camPos", &camPos);
        shader->sendFloat2("camRot", &camRot);
        Math::Mat4 m{};
        shader->sendMat4("viewMat", &m);
    }

    double time = 0;

    while (!window->shouldClose()) {
        const double prevTime = time;
        time = window->getTime();
        const double elapsed = time - prevTime;

        if (mouseFlags) {
            Math::Vec2d tSize = {(double)size.x, (double)size.y};
            if (mouseFlags & HIDE) {
                window->hideCursor(mouseFlags & DRAG);
                window->setCursorPos(tSize / 2);
                mouseFlags &= ~HIDE;
                mousePos = tSize / 2;
            }
            if (mouseFlags & DRAG) {
                window->setCursorPos(tSize / 2); //
            }
        }

        auto camSpeed = camVel.length();

        camVel.y -= gravity * elapsed;
        camPos.y += camVel.y * elapsed;

        if (camPos.y < 2)
            camPos.y = 2, camVel.y = 0, camFlags |= ON_GROUND;
        else if (camPos.y > 4)
            camPos.y = 4, camVel.y = 0;
        if (camPos.x < -4 + CAM_RAD)
            camPos.x = -4 + CAM_RAD;
        else if (camPos.x > 4 - CAM_RAD)
            camPos.x = 4 - CAM_RAD;
        if (camPos.z < -4 + CAM_RAD)
            camPos.z = -4 + CAM_RAD;
        else if (camPos.z > 4 - CAM_RAD)
            camPos.z = 4 - CAM_RAD;

        if (camFlags || camSpeed > 0.001) {
            if (camFlags & 0xF || camSpeed > 0.01) {
                const float s = Math::sin(camRot.x), c = Math::cos(camRot.x);
                if (camFlags & FORWARD) camPos.z += speed * elapsed * c, camPos.x += speed * elapsed * s;
                if (camFlags & BACKWARD) camPos.z -= speed * elapsed * c, camPos.x -= speed * elapsed * s;
                if (camFlags & LEFT) camPos.z += speed * elapsed * s, camPos.x -= speed * elapsed * c;
                if (camFlags & RIGHT) camPos.z -= speed * elapsed * s, camPos.x += speed * elapsed * c;
                shader->sendFloat3("camPos", &camPos);
            }
            if (camFlags & ROTATE) {
                Math::Mat4 viewMat = Math::Mat4::rotation(camRot.y, {1, 0, 0});
                Math::Mat4 tMat = Math::Mat4::rotation(camRot.x, {0, 1, 0});
                viewMat *= tMat;
                shader->sendFloat2("camRot", &camRot);
                shader->sendMat4("viewMat", &viewMat);
                camFlags &= ~ROTATE;
            }
        }

        if (sendUniformFlags) {
            if (sendUniformFlags & SEND_RESIZE) {
                shader->sendFloat2("size", &size);
                sendUniformFlags &= ~SEND_RESIZE;
            }
        }

        shader->sendFloat("time", time);

        Coel::Renderer::clear();
        vao->draw(ibo);
        window->update();
    }
}
