// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

void main()
{
    _out = vec4(vec3(log(1 + texture2D(_texture, _tex_coord).x)), 1);
}
