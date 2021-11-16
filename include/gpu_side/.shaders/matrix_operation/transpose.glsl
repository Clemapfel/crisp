// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _left;

void main()
{
    int row_index = int(textureSize(_left, 0).x * _tex_coord.x);
    int col_index = int(textureSize(_left, 0).y * _tex_coord.y);

    _out = texelFetch(_left, ivec2(col_index, row_index), 0);
}
