// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;
uniform int _n_samples;
uniform vec2 _resolution;

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

    //float y_dist = abs(_tex_coord.y - 0.5 - value);
    _out = vec4(float(abs(_tex_coord.y - value - 0.5) < max(1.f / _resolution.y, clamp(abs(y2 - y1), min(y1, y2), max(y1, y2)))));
}
