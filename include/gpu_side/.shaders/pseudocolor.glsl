// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

vec3 hsv_to_rgb(vec3 rgb)
{
    // source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(rgb.xxx + K.xyz) * 6.0 - K.www);
    return rgb.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), rgb.y);
}

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform float[256] _gray_to_hue; // serialized mapping, -1 means mapped to saturation 0

void main()
{
    // image is grayscale
    float hue = _gray_to_hue[uint(texture(_texture, _tex_coord).r * 255)];
    if (hue == -1)
        _out = vec4(texture(_texture, _tex_coord).rrr, 1);
    else
        _out = vec4(hsv_to_rgb(vec3(hue, 1, 1)), 1);
}
