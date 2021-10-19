#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;
uniform vec2 _texture_size;

// reference used: https://casual-effects.com/research/McGuire2008Median/median5.pix

void s2(inout vec4 a, inout vec4 b)
{
    vec4 temp = a;
    a = min(a, b);
    b = max(temp, b);
}

void t2(int a, int b, inout vec4 v[25])
{
    s2(v[a], v[b]);
}

void t24(int a, int b, int c, int d, int e, int f, int g, int h, inout vec4 v[25])
{
    t2(a, b, v);
    t2(c, d, v);
    t2(e, f, v);
    t2(g, h, v);
}

void t25(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, inout vec4 v[25])
{
    t24(a, b, c, d, e, f, g, h, v);
    t2(i, j, v);
}

void main()
{
    vec2 step_size = 1.f / _texture_size;
    vec4 value[25];

    value[0] = texture(_texture, _tex_coord + (vec2(-2, -2) * step_size));
    value[1] = texture(_texture, _tex_coord + (vec2(-1, -2) * step_size));
    value[2] = texture(_texture, _tex_coord + (vec2(0, -2) * step_size));
    value[3] = texture(_texture, _tex_coord + (vec2(1, -2) * step_size));
    value[4] = texture(_texture, _tex_coord + (vec2(2, -2) * step_size));
    value[5] = texture(_texture, _tex_coord + (vec2(-2, -1) * step_size));
    value[6] = texture(_texture, _tex_coord + (vec2(-1, -1) * step_size));
    value[7] = texture(_texture, _tex_coord + (vec2(0, -1) * step_size));
    value[8] = texture(_texture, _tex_coord + (vec2(1, -1) * step_size));
    value[9] = texture(_texture, _tex_coord + (vec2(2, -1) * step_size));
    value[10] = texture(_texture, _tex_coord + (vec2(-2, 0) * step_size));
    value[11] = texture(_texture, _tex_coord + (vec2(-1, 0) * step_size));
    value[12] = texture(_texture, _tex_coord + (vec2(0, 0) * step_size));
    value[13] = texture(_texture, _tex_coord + (vec2(1, 0) * step_size));
    value[14] = texture(_texture, _tex_coord + (vec2(2, 0) * step_size));
    value[15] = texture(_texture, _tex_coord + (vec2(-2, 1) * step_size));
    value[16] = texture(_texture, _tex_coord + (vec2(-1, 1) * step_size));
    value[17] = texture(_texture, _tex_coord + (vec2(0, 1) * step_size));
    value[18] = texture(_texture, _tex_coord + (vec2(1, 1) * step_size));
    value[19] = texture(_texture, _tex_coord + (vec2(2, 1) * step_size));
    value[20] = texture(_texture, _tex_coord + (vec2(-2, 2) * step_size));
    value[21] = texture(_texture, _tex_coord + (vec2(-1, 2) * step_size));
    value[22] = texture(_texture, _tex_coord + (vec2(0, 2) * step_size));
    value[23] = texture(_texture, _tex_coord + (vec2(1, 2) * step_size));
    value[24] = texture(_texture, _tex_coord + (vec2(2, 2) * step_size));

    t25(0, 1, 3, 4,	2, 4, 2, 3,	6, 7, value);
    t25(5, 7, 5, 6,	9, 7, 1, 7,	1, 4, value);
    t25(12, 13,	11, 13,	11, 12,	15, 16,	14, 16, value);
    t25(14, 15,	18, 19,	17, 19,	17, 18,	21, 22, value);
    t25(20, 22,	20, 21,	23, 24,	2, 5, 3, 6, value);
    t25(0, 6, 0, 3,	4, 7, 1, 7,	1, 4, value);
    t25(11, 14, 8, 14, 8, 11, 12, 15, 9, 15, value);
    t25(9, 12, 13, 16, 10, 16, 10, 13, 20, 23, value);
    t25(17, 23, 17, 20, 21, 24, 18, 24, 18, 21, value);
    t25(19, 22,	8, 17, 9, 18, 0, 18, 0, 9, value);
    t25(10, 19, 1, 19, 1, 10, 11, 20, 2, 20, value);
    t25(2, 11, 12, 21, 3, 21, 3, 12, 13, 22, value);
    t25(4, 22, 4, 13, 14, 23, 5, 23, 5, 14, value);
    t25(15, 24, 6, 24, 6, 15, 7, 16, 7, 19, value);
    t25(3, 11, 5, 17, 11, 17, 9, 17, 4, 10, value);
    t25(6, 12, 7, 14, 4, 6, 4, 7, 12, 14, value);
    t25(10, 14, 6, 7, 10, 12, 6, 10, 6, 17, value);
    t25(12, 17, 7, 17, 7, 10, 12, 18, 7, 12, value);
    t24(10, 18, 12, 20, 10, 20, 10, 12, value);
    gl_FragColor = value[12];
}
