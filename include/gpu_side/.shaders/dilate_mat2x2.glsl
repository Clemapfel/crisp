// generated by /crisp/gpu_side/.code_generation/erode_dilate_matmxn.lua on 22.10.2021, 20:49:11

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform vec2 _texture_size;
uniform mat2 _structuring_element;


void main()
{
    vec2 step_size = 1.f / _texture_size;
    vec4 max_v = vec4(0);

    max_v = max(max_v,_structuring_element[0][0] * texture(_texture, _tex_coord + (vec2(-1, -1) * step_size)));
    max_v = max(max_v,_structuring_element[0][1] * texture(_texture, _tex_coord + (vec2(-1, 0) * step_size)));
    max_v = max(max_v,_structuring_element[1][0] * texture(_texture, _tex_coord + (vec2(0, -1) * step_size)));
    max_v = max(max_v,_structuring_element[1][1] * texture(_texture, _tex_coord + (vec2(0, 0) * step_size)));

    _out = max_v;
}