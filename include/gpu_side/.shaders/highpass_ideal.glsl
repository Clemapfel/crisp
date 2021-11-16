// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

uniform float _pass_factor;
uniform float _reject_factor;
uniform float _cutoff;

uniform vec2 _offset;

void main()
{
    vec2 pos = _tex_coord;
    vec2 texture_size = textureSize(_texture, 0);
    float to_square = texture_size.y / texture_size.x;
    pos.y *= to_square;

    float dist_left = distance(pos, vec2(0.5, 0.5 * to_square) + vec2(_offset.x, -1 * _offset.y));
    float dist_right = distance(pos, vec2(0.5, 0.5 * to_square) + vec2(-1 * _offset.x, _offset.y));

    float factor;
    if (dist_left < _cutoff || dist_right < _cutoff)
        factor = _reject_factor;
    else
        factor = _pass_factor;

    _out = texture(_texture, _tex_coord) * factor;
}
