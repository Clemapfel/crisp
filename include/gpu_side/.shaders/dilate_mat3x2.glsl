// Copyright 2021 Clemens Cords (mail@clemens-cords.com)
// generated by /crisp/gpu_side/.code_generation/erode_dilate_matmxn.lua on 09.11.2021, 17:25:10

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform mat2x3 _structuring_element;


void main()
{
    vec2 step_size = 1.f / textureSize(_texture, 0);
    vec4 max_v = vec4(0);

    max_v = max(max_v,_structuring_element[0][0] * texture(_texture, _tex_coord + (vec2(-1, -1) * step_size)));
    max_v = max(max_v,_structuring_element[0][1] * texture(_texture, _tex_coord + (vec2(-1, 0) * step_size)));
    max_v = max(max_v,_structuring_element[0][2] * texture(_texture, _tex_coord + (vec2(-1, 1) * step_size)));
    max_v = max(max_v,_structuring_element[1][0] * texture(_texture, _tex_coord + (vec2(0, -1) * step_size)));
    max_v = max(max_v,_structuring_element[1][1] * texture(_texture, _tex_coord + (vec2(0, 0) * step_size)));
    max_v = max(max_v,_structuring_element[1][2] * texture(_texture, _tex_coord + (vec2(0, 1) * step_size)));

    _out = max_v;
}