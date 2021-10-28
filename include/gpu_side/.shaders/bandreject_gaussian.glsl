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
uniform float _cutoff_a;
uniform float _cutoff_b;

uniform vec2 _offset;

void main()
{
    vec2 pos = _tex_coord;
    float to_square = _texture_size.y / _texture_size.x;
    pos.y *= to_square;

    float dist_left = distance(pos, vec2(0.5, 0.5 * to_square) + vec2(_offset.x, -1 * _offset.y));
    float dist_right = distance(pos, vec2(0.5, 0.5 * to_square) + vec2(-1 * _offset.x, _offset.y));

    float upper = _cutoff_a;
    float lower = _cutoff_b;
    float width = upper - lower;
    float center = lower + width / 2.f;

    float value_left = (dist_left*dist_left - center*center) / (dist_left*width);
    float factor_left = 1 - project(_reject_factor, _pass_factor, exp(-1 * value_left*value_left));

    float value_right = (dist_right*dist_right - center*center) / (dist_right*width);
    float factor_right = 1 - project(_reject_factor, _pass_factor, exp(-1 * value_right*value_right));

    _out = texture(_texture, _tex_coord) * max(factor_left, factor_right);
}
