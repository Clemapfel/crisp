// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform vec4 _min;
uniform vec4 _max;

void main()
{
    _out = (texture2D(_texture, _tex_coord) - _min) / (_max - _min);
}
