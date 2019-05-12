#version 440 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 posSize;
layout (location = 2) in vec4 col;
uniform mat4 view_mat;

out vec4 v_pixPos;
out vec4 v_relPos;
out vec4 v_posSize;
out vec4 v_col;

void main()
{
	v_pixPos = vec4(pos.xy, 0, 0);
	v_relPos = vec4(pos.xy - posSize.xy, 0, 0);
	v_posSize = posSize;
	v_col = col;
	gl_Position = vec4(pos.xy, 0, 1) * view_mat;
}