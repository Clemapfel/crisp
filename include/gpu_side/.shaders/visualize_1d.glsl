// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;
uniform sampler2D _texture;

void main()
{
    _out = texture2D(_texture, _tex_coord.xy); //(texture(_texture_1d, _tex_coord.x).xxxx + 25600) / (24832 + 25600);
}
