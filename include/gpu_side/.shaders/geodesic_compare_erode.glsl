#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform sampler2D _original;
uniform sampler2D _mask;

void main()
{
    _out = max(texture2D(_texture, _tex_coord), texture2D(_mask, _tex_coord) * texture2D(_original, _tex_coord));
}
