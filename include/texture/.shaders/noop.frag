#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture_0;
uniform sampler2D _texture_1;

void main()
{
    if (_tex_coord.x > 0.5)
        gl_FragColor = texture2D(_texture_0, _tex_coord);
    else
        gl_FragColor = texture2D(_texture_1, _tex_coord);
}
