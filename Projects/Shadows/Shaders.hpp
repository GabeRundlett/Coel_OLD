#pragma once

namespace Scene {
	static constexpr const char *const vertSrc = R"(
	#version 440

	layout (location = 0) in vec3 pos;
	layout (location = 1) in vec3 nrm;
	layout (location = 2) in vec2 tex;
	layout (location = 3) in vec4 col;
	out vec4 vPos;
	out vec3 vNrm;
	out vec2 vTex;
	out vec4 vCol;
	out vec4 vPosLightSpace;

	uniform mat4 lightProjMat;
	uniform mat4 lightViewMat;

	uniform mat4 projMat;
	uniform mat4 viewMat;
	uniform mat4 modelMat;

	void main() {
		vCol = col, vTex = tex, vNrm = nrm;
		vPos = modelMat * vec4(pos, 1);
		vPosLightSpace = lightProjMat * lightViewMat * vPos;
		gl_Position = projMat * viewMat * vPos;
	}
	)";

	static constexpr const char *const fragSrc = R"(
	#version 440

	in vec4 vPos;
	in vec3 vNrm;
	in vec2 vTex;
	in vec4 vCol;
	in vec4 vPosLightSpace;
	out vec4 color;

	uniform vec3 lightDir;
	uniform vec3 viewPos;
	uniform sampler2D shadowMap;

	void main() {
		float nrmDotLightDir = dot(normalize(vNrm), lightDir);

		vec3 projCoords = vPosLightSpace.xyz / vPosLightSpace.w;
		projCoords = projCoords * 0.5 + 0.5;

	    float closestDepth = texture(shadowMap, projCoords.xy).r;
	    float currentDepth = projCoords.z;
		float bias = max(0.05 * (1.0 - nrmDotLightDir), 0.005);
	    float shade = 1;
		if (projCoords.x > 1) {
			color += vec4(1, 0, 0, 1) / 5;
		} else if (projCoords.x < 0) {
			color += vec4(0, 1, 0, 1) / 5;
		} else if (projCoords.y > 1) {
			color += vec4(0, 1, 0, 1) / 5;
		} else if (projCoords.y < 0) {
			color += vec4(0, 1, 0, 1) / 5;
		} else if (projCoords.z > 1) {
			color += vec4(0, 0, 1, 1) / 5;
		} else if (projCoords.z < 0) {
			color += vec4(0, 0, 1, 1) / 5;
		} else if (currentDepth - bias > closestDepth)
			shade = 0;

		float ambient = 0.1;
		float diff = clamp(nrmDotLightDir, 0, 1);

		color = vCol * max(shade * (diff + 0), ambient);
		color.w = 1;
	}
	)";
}

namespace Shadow {
    static constexpr const char *const vertSrc = R"(
	#version 440

	layout (location = 0) in vec3 pos;

	uniform mat4 projMat;
	uniform mat4 viewMat;
	uniform mat4 modelMat;

	void main() {
		gl_Position = projMat * viewMat * modelMat * vec4(pos, 1);
	}
	)";

    static constexpr const char *const fragSrc = R"(
	#version 440

	// out vec4 col;

	void main() {
		// col = vec4(vec3(gl_FragCoord.z), 1);
		// gl_FragDepth = gl_FragCoord.z;
	}
	)";
}

namespace Quad {
	static constexpr const char *const vertSrc = R"(
	#version 440

	layout (location = 0) in vec2 pos;
	layout (location = 1) in vec2 tex;

	out vec2 vTex;

	void main() {
		vTex = tex;
		gl_Position = vec4(pos, 0, 1);
	}
	)";

	static constexpr const char *const fragSrc = R"(
	#version 440

	in vec2 vTex;
	out vec4 color;

	uniform sampler2D tex;

	void main() {
		color = texture(tex, vTex);
		color.w = 1;
	}
	)";
}