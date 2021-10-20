#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;

void main()
{
    gl_FragData[0] = vec4(vec3(mod(_tex_coord.x + _tex_coord.y * 1000000, 2) == 0.f), 1);
}
