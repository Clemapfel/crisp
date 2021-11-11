// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _left;
uniform sampler2D _right;

void main()
{
    int row_index = int(textureSize(_left, 0).x * _tex_coord.x);
    int col_index = int(textureSize(_right, 0).y * _tex_coord.y);

    float value = 0;

    for (int i = 0; i < textureSize(_left, 0).y; ++i)
        value += texelFetch(_left, ivec2(row_index, i), 0).x * texelFetch(_right, ivec2(i, col_index), 0).x;

    _out = vec4(value);
}
