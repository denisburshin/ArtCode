#version 460

layout (location = 0) out vec4 color;

uniform sampler2D u_texture;

in vec2 t_pos;

void main()
{
	color = texture(u_texture, t_pos);
}