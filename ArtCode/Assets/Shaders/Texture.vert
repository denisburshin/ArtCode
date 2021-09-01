#version 460

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 tex_pos;

uniform mat4 u_ProjectionView;
uniform mat4 u_ModelMatrix;

out vec2 t_pos;

void main()
{
	t_pos = tex_pos;
	gl_Position = u_ProjectionView * u_ModelMatrix * vec4(v_pos, 1.0);
}