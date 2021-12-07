// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1DArray _signal_array;
uniform int _n_signals;

void main()
{
    vec2 pos = _tex_coord;
    float value = texture(_signal_array, vec2(pos.x, pos.y * _n_signals)).x;
    _out = vec4(vec3(value), 1);
}