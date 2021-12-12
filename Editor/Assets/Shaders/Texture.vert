#version 460

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec2 tex_pos;
layout (location = 3) in float tex_index;
layout (location = 4) in int entity_id;

uniform mat4 u_ProjectionView;
uniform mat4 u_ModelMatrix;

out vec4 v_Color;
out vec2 t_pos;
out float tex_Index;
out flat int entityID;

void main()
{
	v_Color = v_color;
	t_pos = tex_pos;
	tex_Index = tex_index;
	entityID = entity_id;
	gl_Position = u_ProjectionView * u_ModelMatrix * vec4(v_pos, 1.0);
}