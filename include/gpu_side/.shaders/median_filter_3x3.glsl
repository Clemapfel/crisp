#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;

// reference used: https://casual-effects.com/research/McGuire2008Median/median.pix

void s2(inout vec4 a, inout vec4 b)
{
    vec4 temp = a;
    a = min(a, b);
    b = max(temp, a);
}

void mn3(inout vec4 a, inout vec4 b, inout vec4 c)
{
    s2(a, b);
    s2(a, c);
}

void mx3(inout vec4 a, inout vec4 b, inout vec4 c)
{
    s2(b, c);
    s2(a, c);
}

void mnmx3(inout vec4 a, inout vec4 b, inout vec4 c)
{
    mx3(a, b, c); 
    s2(a, b); 
}

void mnmx4(inout vec4 a, inout vec4 b, inout vec4 c, inout vec4 d)
{
    s2(a, b);
    s2(c, d);
    s2(a, c);
    s2(b, d);
}

void mnmx5(inout vec4 a, inout vec4 b, inout vec4 c, inout vec4 d, inout vec4 e)
{
    s2(a, b);
    s2(c, d);
    mn3(a, c, e);
    mx3(b, d, e);
}

void mnmx6(inout vec4 a, inout vec4 b, inout vec4 c, inout vec4 d, inout vec4 e, inout vec4 f)
{
    s2(a, d);
    s2(b, e);
    s2(c, f);
    mn3(a, b, c);
    mx3(d, e, f);
}

void main()
{
    vec2 step_size = 1.f / textureSize(_texture, 0);
    vec4 value[9];

    value[0] = texture(_texture, _tex_coord + (vec2(-1, -1) * step_size));
    value[1] = texture(_texture, _tex_coord + (vec2(0, -1) * step_size));
    value[2] = texture(_texture, _tex_coord + (vec2(1, -1) * step_size));
    value[3] = texture(_texture, _tex_coord + (vec2(-1, 0) * step_size));
    value[4] = texture(_texture, _tex_coord + (vec2(0, 0) * step_size));
    value[5] = texture(_texture, _tex_coord + (vec2(1, 0) * step_size));
    value[6] = texture(_texture, _tex_coord + (vec2(-1, 1) * step_size));
    value[7] = texture(_texture, _tex_coord + (vec2(0, 1) * step_size));
    value[8] = texture(_texture, _tex_coord + (vec2(1, 1) * step_size));

    mnmx6(value[0], value[1], value[2], value[3], value[4], value[5]);
    mnmx5(value[1], value[2], value[3], value[4], value[6]);
    mnmx4(value[2], value[3], value[4], value[7]);
    mnmx3(value[3], value[4], value[8]);

    gl_FragColor = value[4];
}
