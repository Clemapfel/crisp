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

    vec2 center = vec2(0.5, 0.5 * to_square) + vec2(_offset.x, -1 * _offset.y);
    float upper = max(_cutoff_a, _cutoff_b);
    float lower = min(_cutoff_a, _cutoff_b);
    float dist = distance(center, pos);

    if (dist >= lower && dist <= upper)
        _out = vec4(vec3(_pass_factor), 1);
    else
        _out = vec4(vec3(_reject_factor), 1);
}
