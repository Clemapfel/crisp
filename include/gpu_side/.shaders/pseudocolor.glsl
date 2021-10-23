#version 330 core

vec3 hsv_to_rgb(vec3 rgb)
{
    // source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

uniform float[256] _gray_to_hue;

void main()
{
    // image is grayscale
    vec3 as_hsv = vec3(_gray_to_hue[texture(_texture, _tex_coord).r * 256], 1, 1);
    _out = vec4(hsv_to_rgb(as_hsv), 1);
}
