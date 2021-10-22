#version 330 core

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;
uniform vec2 _texture_size;
uniform mat3 _structuring_element;

float to_value(int x, int y)
{
    // -1 -> infinity
    //  0 -> infinity
    //  1 -> 1



    if (_structuring_element[x][y])
     texture(_texture, _tex_coord + (vec2(x - 1, y - 1) * step_size));
}

void main()
{
    float min_v = 1;

    min_v = min(float(uint(_structuring_element[0][0])) )

    _out = texture2D(_texture, _tex_coord);
}
