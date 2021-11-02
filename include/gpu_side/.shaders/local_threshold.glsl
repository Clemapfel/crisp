// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform vec2 _texture_size;

uniform uint _neighborhood_size; // spreads sample net, recommended: {1, 2, 3, ..., 25}
uniform uint _correction; // the higher the less noisy but also loss of detail, recommended: {0, 1, ..., 4}

void main()
{
    vec2 step_size = 1.f / _texture_size;
    vec4 mean = vec4(0);
    int i = int(_neighborhood_size);

    mean += texture(_texture, _tex_coord + vec2( 0 * i, -7 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i, -3 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i, -1 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i,  1 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i,  3 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i,  7 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-7 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-3 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-1 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 0 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 1 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 3 * i,  0 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 7 * i,  0 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-1 * i, -1 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-1 * i,  1 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 1 * i, -1 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 1 * i,  1 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-2 * i, -2 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-2 * i,  2 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 2 * i, -2 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 2 * i,  2 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-2 * i, -4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-4 * i, -4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-4 * i, -2 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2( 2 * i, -4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-4 * i, -4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-4 * i,  2 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2( 2 * i,  4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 4 * i,  4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 4 * i,  2 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-2 * i,  4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 4 * i,  4 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 4 * i, -2 * i) * step_size);

    mean += texture(_texture, _tex_coord + vec2(-3 * i, -6 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2(-3 * i,  6 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 3 * i, -6 * i) * step_size);
    mean += texture(_texture, _tex_coord + vec2( 3 * i,  6 * i) * step_size);

    mean /= 38 + 1 + float(_correction);

    vec4 result;
    result.x = float(texture(_texture, _tex_coord).x > mean.x);
    result.y = float(texture(_texture, _tex_coord).y > mean.y);
    result.z = float(texture(_texture, _tex_coord).z > mean.z);
    result.w = float(texture(_texture, _tex_coord).w > mean.w);

    _out = result;
}
