#pragma once

static constexpr const char *const vertSrc = R"(
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;
layout (location = 2) in vec2 aTex;
uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modlMat;
out vec3 vPos;
out vec3 vNrm;
out vec2 vTex;
void main() {
	vPos = (modlMat * vec4(aPos, 1)).xyz;
	vNrm = inverse(transpose(mat3(modlMat))) * aNrm;
	vTex = aTex;
	gl_Position = projMat * viewMat * vec4(vPos, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 450 core
in vec3 vPos;
in vec3 vNrm;
in vec2 vTex;
out vec4 fragColor;

uniform vec3 camPos;
uniform vec3 objectCol;

uniform vec3 lightCol;
uniform vec3 lightDir;
float lightConstant = 1;
float lightLinear = 0.09;
float lightQuadratic = 0.032;

uniform vec3 ambientCol;
uniform float ambientStrength;

void main() {
	// vec3 lightDir = lightPos - vPos;
	// float lightDist = length(lightDir);
	// lightDir /= lightDist;

	vec3 viewDir = camPos - vPos;
	float viewDist = length(viewDir);
	viewDir /= viewDist;
	
	vec3 diffuse = lightCol * max(dot(vNrm, lightDir), 0);
	vec3 reflectDir = reflect(-lightDir, vNrm);
	vec3 specular =  lightCol * pow(max(dot(viewDir, reflectDir), 0), 32) * 0.5;
	vec3 ambient = lightCol * ambientStrength;
	float attenuation = 1; //1.f / (lightConstant + lightLinear * lightDist + lightQuadratic * lightDist * lightDist);

	fragColor = vec4((diffuse + specular + ambient) * objectCol * attenuation, 1);
}
)";
