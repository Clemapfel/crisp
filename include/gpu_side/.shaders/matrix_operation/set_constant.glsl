// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform float _scalar;

void main()
{
    _out = vec4(_scalar);
}
