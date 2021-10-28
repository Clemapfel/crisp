// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

float project(float lower_bound, float upper_bound, float value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform vec2 _texture_size;

uniform float _pass_factor;
uniform float _reject_factor;
uniform float _cutoff;

uniform vec2 _offset;
uniform int _order;

void main()
{
    vec2 pos = _tex_coord;
    float to_square = _texture_size.y / _texture_size.x;
    pos.y *= to_square;

    float dist = distance(pos, vec2(0.5, 0.5 * to_square) + vec2(_offset.x, -1 * _offset.y));
    float factor = project(_reject_factor, _pass_factor, 1.f / (1 + pow(dist / _cutoff, 2 * _order)));

    _out = texture(_texture, _tex_coord) * factor;
}
