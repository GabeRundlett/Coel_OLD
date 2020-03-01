#pragma once

static constexpr const char *const vertSrc = R"(
#version 440

layout (location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440

uniform vec3 viewPos;
uniform vec3 viewRot;
uniform vec2 viewSize;
out vec4 color;

float viewAspect = viewSize.x / viewSize.y;
vec2 uv = (gl_FragCoord.xy / viewSize - 0.5f) * 2.f;

mat3 rotX(in float angle) {
	float c = cos(angle), s = sin(angle);
	return mat3(
		1, 0,  0,
		0, c, -s,
		0, s,  c);
}

mat3 rotY(in float angle) {
	float c = cos(angle), s = sin(angle);
	return mat3(
		c, 0, -s,
		0, 1,  0,
		s, 0,  c);
}

mat3 rotZ(in float angle) {
	float c = cos(angle), s = sin(angle);
	return mat3(
		c, -s, 0,
		s,  c, 0,
		0,  0, 1);
}

#define MAX_DIST 1000.f
#define MIN_DIST 0.001f
#define MAX_ITER 1000.f
#define RECURSE_COUNT 1.f

struct PointLight {
	vec3 pos;
	float strength;
};

#define POINT_LIGHT_COUNT 1
PointLight pointLights[POINT_LIGHT_COUNT];

float sd_planeXY(in vec3 origin, in float z) {
	return origin.z - z;
}

float sd_ellipsoid(in vec3 origin, in vec3 pos, in vec3 size) {
	return length(pos - origin) - 1.f;
}

float sd_rectPrism(in vec3 origin, in vec3 pos, in vec3 size) {
	return length(max(abs(pos - origin) - size, 0.f));
}

void smin(inout float a, in float b, in float k) {
	float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    a = mix(b, a, h) - k * h * (1.0 - h);
}

float sd_scene(in vec3 origin) {
	float minDist = MAX_DIST, tempDist = MAX_DIST;

	tempDist = sd_planeXY(origin, -1);
	minDist = min(minDist, tempDist);

	tempDist = sd_ellipsoid(origin, vec3(0, 2, 0), vec3(1));
	minDist = min(minDist, tempDist);

	tempDist = sd_rectPrism(origin, vec3(0, 0, -1), vec3(4, 4, 0.1));
	minDist = min(minDist, tempDist);

	tempDist = sd_rectPrism(origin, vec3(2, -1, -0.5), vec3(0.5));
	minDist = min(minDist, tempDist);

	tempDist = sd_ellipsoid(origin, vec3(-2, 0, -1), vec3(1));
	minDist = min(minDist, tempDist);

	return minDist;
}

vec3 getNormal(in vec3 origin) {
	vec2 e = vec2(0.001, 0);
	float dist = sd_scene(origin);
	return normalize(vec3(
		dist - sd_scene(origin - e.xyy),
		dist - sd_scene(origin - e.yxy),
		dist - sd_scene(origin - e.yyx)
	));
}

bool march(in vec3 ray, inout vec3 origin, inout float minDist, inout float totalDist, inout float iter) {
	for (; iter < MAX_ITER; ++iter) {
		float dist = sd_scene(origin);
		
		if (dist < MIN_DIST) return true;
		if (totalDist > MAX_DIST) break;
		if (dist < minDist) minDist = dist;
		
		origin += ray * dist, totalDist += dist;
	}

	return false;
}

float marchScene(in vec3 ray, in vec3 origin) {
	float diffuse = 0;
	
	for (float count; count < RECURSE_COUNT; ++count) {
		float minDist = MAX_DIST, iter = 0, totalDist = 0;
		if (!march(ray, origin, minDist, totalDist, iter)) break;

		vec3 normal = getNormal(origin);
		
		for (int i = 0; i < POINT_LIGHT_COUNT; ++i) {
			PointLight l = pointLights[i];
			vec3 orig = origin + normal * MIN_DIST * 1.2f;

			float lDist = length(l.pos - orig);
			vec3 lightRay = normalize(l.pos - orig);
			minDist = lDist, totalDist = 0, iter = 0;

			if (!march(lightRay, orig, minDist, totalDist, iter)) 
				diffuse += clamp(dot(lightRay, normal) / lDist / lDist, 0, 1) * 15;
		}

		ray += normal * dot(normal, ray) * 2;
	}

	return diffuse;
}

void main() {
	uv.x *= viewAspect;

	vec3 ray = normalize(vec3(uv.x, 1.f, uv.y) * rotX(viewRot.x) * rotZ(viewRot.z));
	vec3 origin = viewPos;
	float iter = 0.f, totalDist = 0.f;

	pointLights[0].pos = vec3(0, 0, 5);
	pointLights[0].strength = 20.f;

	float s = marchScene(ray, origin);

	color = vec4(vec3(s), 1);
}
)";
