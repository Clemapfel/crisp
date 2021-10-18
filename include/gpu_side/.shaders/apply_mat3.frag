#version 330 core

float project(float lower_bound, float upper_bound, float value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

in vec2 _tex_coord;

uniform mat3 _kernel;
uniform sampler2D _texture;
uniform float _max;

void main()
{
    mat3 kernel = mat3(-1);
    kernel[0][0] = 8;

    float min = 0;
    float max = 0;
    float value = 0;
    for (int s = -1; s <= +1; ++s)
    {
        for (int t = -1; t <= +1; ++t)
        {
            float kernel_val = kernel[1 + s][1 + t];

            value += texture(_texture, _tex_coord + vec2(s, t)).x * kernel_val;

            if (kernel_val < 0)
                min += kernel_val;
            else
                max += kernel_val;
        }
    }

    value -= min;
    value /= (-1 * min + max);

    gl_FragColor = vec4(value, value, value, 1); ////texture2D(_texture, _tex_coord); //vec4(vec3(_kernel * neighborhood), 1);
}
