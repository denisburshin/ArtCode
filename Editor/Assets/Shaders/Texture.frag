#version 460

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 t_pos;
in float tex_Index;

uniform sampler2D u_texture[32];

void main()
{
	color = texture(u_texture[int(tex_Index)], t_pos) * v_Color;
}