// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

void main()
{
    _out = clamp(texture2D(_texture, _tex_coord), vec4(0), vec4(1));
}
