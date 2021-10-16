#version 330 core

layout (location = 0) in vec3 _vertex_pos;
layout (location = 1) in vec3 _vertex_color_rgb;
layout (location = 2) in vec2 _vertex_tex_coord;

out vec2 _tex_coord;

// DO NOT MODIFY

void main()
{
    gl_Position = vec4(_vertex_pos, 1.0);
    _tex_coord = _vertex_tex_coord;
}