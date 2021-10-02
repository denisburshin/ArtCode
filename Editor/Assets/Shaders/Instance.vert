#version 460

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 offset;

void main()
{
    gl_Position = vec4(v_pos + offset, 1.0);
}