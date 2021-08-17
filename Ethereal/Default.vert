#version 460

layout (location = 0) in vec3 v_pos;

uniform mat4 u_ProjectionView;

void main()
{
	gl_Position = u_ProjectionView * vec4(v_pos, 1.0);
}