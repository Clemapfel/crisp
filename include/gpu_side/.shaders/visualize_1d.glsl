// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;

void main()
{
    float value = texture(_texture_1d, _tex_coord.x).x * 0.5;

    float y_distance = _tex_coord.y - 0.5;

    if (abs(y_distance - value) < 1/500.f)
        _out = vec4(1);
    else
        _out = vec4(0);
}
