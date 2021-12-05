// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;
uniform int _n_samples;

void main()
{
    float one_px = 1.f / float(_n_samples);

    float x = _tex_coord.x;
    float x1 = x - mod(x, one_px);
    float x2 = x + one_px - mod(x, one_px);
    float y1 = texture(_texture_1d, x1).x;
    float y2 = texture(_texture_1d, x2).x;

    float slope = (y2 - y1) / (x2 - x1);
    float intercept = -1 * (slope * x1 - y1);

    float value = (slope * x + intercept);
    value = (value) * 0.5;

    float y_dist = _tex_coord.y - 0.5;
    _out = vec4(float((y_dist > 0 && y_dist < value) || (y_dist < 0 && y_dist > value)));
}
