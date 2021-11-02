// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform float _threshold;

void main()
{
    vec4 result;
    result.x = float(texture2D(_texture, _tex_coord).x < _threshold);
    result.y = float(texture2D(_texture, _tex_coord).y < _threshold);
    result.z = float(texture2D(_texture, _tex_coord).z < _threshold);
    result.w = float(texture2D(_texture, _tex_coord).w < _threshold);

    _out = result;
}
