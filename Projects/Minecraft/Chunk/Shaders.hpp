#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
out VData {
    vec2 tex;
    vec3 nrm;
} vdata;

void main() {
	vec2 texSize = vec2(60, 80);
	vdata.tex = ((tex) * 16 + 2) / texSize;
	gl_Position = vec4(pos, 1);
}
)";

static constexpr const char *const geomSrc = R"(
#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;
in VData {
    vec2 tex;
    vec3 nrm;
} vdata[];
out GData {
    vec2 tex;
    vec3 nrm;
} gdata;

uniform mat4 viewMat;

void main() {
	vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
	vec3 n = normalize(cross(b, a));
	for (int i = 0; i < 3; ++i) {
        gl_Position = viewMat * gl_in[i].gl_Position;
		gdata.tex = vdata[i].tex;
		gdata.nrm = n;
		EmitVertex();
	}
	EndPrimitive();
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

in GData {
    vec2 tex;
    vec3 nrm;
} gdata;
layout (binding = 0) uniform sampler2D tex;
out vec4 color;

void main() {
	vec3 sunDir = normalize(vec3(1, -3, 2));
	float diffuse = dot(gdata.nrm, sunDir);
	diffuse = clamp(diffuse, 0.5f, 1.f);

	color = vec4(gdata.nrm, 1);
	color = texture(tex, gdata.tex);

	color.xyz *= diffuse;
}
)";
