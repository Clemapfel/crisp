// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler1D _texture_1d;
uniform int _n_samples;

vec3 hsv_to_rgb(vec3 c)
{
    // source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float value = texture(_texture_1d, _tex_coord.y).x;
    _out = vec4(vec3(value), 1); //hsv_to_rgb(vec3(value, 1, 1)), 1);
}