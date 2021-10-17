#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;

void main()
{
    gl_FragColor = vec4(0.4, 1, 1, 1); //texture2D(_texture, _tex_coord);
}
