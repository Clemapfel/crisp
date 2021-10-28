// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

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

    vec2 center_left = vec2(0.5, 0.5 * to_square) + vec2(_offset.x, -1 * _offset.y);
    vec2 center_right = vec2(0.5, 0.5 * to_square) + vec2(-1 * _offset.x, _offset.y);

    float upper = max(_cutoff_a, _cutoff_b);
    float lower = min(_cutoff_a, _cutoff_b);

    float dist_left = distance(center_left, pos);
    float dist_right = distance(center_right, pos);

    float factor;
    if ((dist_left >= lower && dist_left <= upper) || (dist_right >= lower && dist_right <= upper))
        factor = _pass_factor;
    else
        factor = _reject_factor;

    _out = texture(_texture, _tex_coord) * factor;
}
