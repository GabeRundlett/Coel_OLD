#version 440 core

in vec4 v_pixPos;
in vec4 v_relPos;
in vec4 v_posSize;
in vec4 v_col;

out vec4 Color;

void main()
{
	double rounding = 15 * 2;
	rounding = min(v_posSize.z / 2, min(v_posSize.w / 2, rounding));
	
	// shadow
	double val;

	double edge0 = length(v_relPos.xy - vec2(rounding)), 
		edge1 = length(v_relPos.xy - vec2(rounding, v_posSize.w - rounding)),
		edge2 = length(v_relPos.xy - vec2(v_posSize.z - rounding, rounding)),
		edge3 = length(v_relPos.xy - vec2(v_posSize.z - rounding, v_posSize.w - rounding));

	if (v_relPos.x < rounding) // left
	{
		if (v_relPos.y < rounding) // top
			val = rounding - edge0;
		else if (v_relPos.y > v_posSize.w - rounding) // bottom
			val = rounding - edge1;
		else // middle
			val = v_relPos.x;
	}
	else if (v_relPos.x > v_posSize.z - rounding) // right
	{
		if (v_relPos.y < rounding) // top
			val = rounding - edge2;
		else if (v_relPos.y > v_posSize.w - rounding) // bottom
			val = rounding - edge3;
		else // middle
			val = v_posSize.z - v_relPos.x;
	}
	else // middle
	{
		if (v_relPos.y < rounding) // top
			val = v_relPos.y;
		else if (v_relPos.y > v_posSize.w - rounding) // bottom
			val = v_posSize.w - v_relPos.y;
		else // middle
			val = rounding;
	}

	val /= rounding;
	val -= (1 - v_relPos.y / v_posSize.w) * 0.05;
	Color = vec4(0, 0, 0, val / 6);

	double rounding1 = 13;
	val = clamp(-length(v_relPos.xy - vec2(rounding1, rounding1)) + rounding1 - rounding / 2, 0, 1);
	val += clamp(-length(v_relPos.xy - vec2(rounding1, v_posSize.w - rounding1)) + rounding1 - rounding / 2, 0, 1);
	val += clamp(-length(v_relPos.xy - vec2(v_posSize.z - rounding1, rounding1)) + rounding1 - rounding / 2, 0, 1);
	val += clamp(-length(v_relPos.xy - vec2(v_posSize.z - rounding1, v_posSize.w - rounding1)) + rounding1 - rounding / 2, 0, 1);

	if (v_relPos.x > rounding1 + 1 && v_relPos.x < v_posSize.z - rounding1 - 1)
		val = 1;
	else if (v_relPos.y > rounding1 && v_relPos.y < v_posSize.w - rounding1)
		val = 1;

	if (v_relPos.x > rounding / 2 && v_relPos.x < v_posSize.z - rounding / 2 && v_relPos.y > rounding / 2 && v_relPos.y < v_posSize.w - rounding / 2 && val > .01)
		Color = vec4(v_col.xyz, val * 1.5);

}