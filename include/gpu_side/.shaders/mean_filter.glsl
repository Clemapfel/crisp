#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;
uniform vec2 _texture_size;

uniform int _neighborhood_size;

void main()
{
    vec4 value = vec4(0);

    vec2 step_size = 1.f / _texture_size;
    int half_n = int(_neighborhood_size / 2.f + 1);

    for (int i = 0; i < _neighborhood_size; ++i)
        for (int j = 0; j < _neighborhood_size; ++j)
            value += texture(_texture, _tex_coord + (vec2(half_n - i, half_n - j) * step_size));

    gl_FragColor =value.x / float(_neighborhood_size * _neighborhood_size);
}
