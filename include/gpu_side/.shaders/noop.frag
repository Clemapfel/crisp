#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;

// DO NOT MODIFY

void main()
{
    gl_FragColor = texture2D(_texture, _tex_coord);
}
