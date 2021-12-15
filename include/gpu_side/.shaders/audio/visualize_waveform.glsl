// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;
uniform int _n_samples;

void main()
{
    float value = texture(_texture_1d, _tex_coord.x).x * 0.5;
    float y_distance = abs(_tex_coord.y - 0.5);
    _out = vec4(float((y_distance < 0 && y_distance > value) || (y_distance >= 0 && y_distance < value)));
}
