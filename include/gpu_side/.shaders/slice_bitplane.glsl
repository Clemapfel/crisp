// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform uint _bitplane;

void main()
{
    // quantize into [0, 255], then treat last 8-bit of uint as if it was a char and compute bitplane
    vec4 img = texture2D(_texture, _tex_coord);
    float val = (img.x + img.y + img.z) / 3.f;
    uint pseudo_char = uint(val * 255.f);
    uint bit = (pseudo_char >> _bitplane) & 1u;

    _out = vec4(bit);
}
