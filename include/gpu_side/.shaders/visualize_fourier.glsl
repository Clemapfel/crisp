// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform float _min;
uniform float _max;

void main()
{
    float value = log(1 + texture(_texture, _tex_coord).x);
    value -= _min;
    value /= (_max + _min);
    _out = vec4(vec3(value), 1);
}
