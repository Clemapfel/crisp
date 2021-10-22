#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

void main()
{
    _out = texture2D(_texture, _tex_coord);
}
