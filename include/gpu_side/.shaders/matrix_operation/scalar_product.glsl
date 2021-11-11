// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _left;
uniform float _scalar;

void main()
{
    float value = texelFetch(_left, ivec2(textureSize(_left, 0) * _tex_coord), 0).x * _scalar;
    _out = vec4(value);
}
