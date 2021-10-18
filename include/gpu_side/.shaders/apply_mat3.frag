#version 330 core

in vec2 _tex_coord;

uniform mat3 _kernel;
uniform sampler2D _texture;

void main()
{
    mat3 neighborhood;

    for (int x = -1; x <= 1; x = x + 1)
    {
        for (int y = -1; y <= 1; y = y + 1)
        {
            vec3 val = texture2D(_texture, _tex_coord + vec2(x, y)).xyz;
            neighborhood[1 + x][1 + y] = (val.x + val.y + val.z) / 3.f;
        }
    }

    gl_FragColor = vec4(vec3(_kernel * neighborhood), 1);
}
