// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;

void main()
{
    float value = texture(_texture_1d, _tex_coord.x).x;
    value = smoothstep(0, 1, value);
    _out = vec4(vec3(value), 1.0); //(texture(_texture_1d, _tex_coord.x).xxxx + 25600) / (24832 + 25600);
}
