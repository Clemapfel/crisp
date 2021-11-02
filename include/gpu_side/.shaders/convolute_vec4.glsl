#version 330 core

in vec2 _tex_coord;

uniform sampler2D _texture;
uniform vec2 _texture_size;

uniform vec4 _kernel;
uniform bool _vertical;

void main()
{
    vec2 step_size = 1 / _texture_size;

    float kernel_sum = 0;
    vec4 response = vec4(0);
    float kernel_val;

    kernel_val = _kernel[0];
    if (_vertical)
        response += kernel_val * texture(_texture, _tex_coord + (vec2(0, -1) * step_size));
    else
        response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, 0) * step_size));

    kernel_sum += kernel_val;

    kernel_val = _kernel[1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 0) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[2];
    if (_vertical)
        response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 1) * step_size));
    else
        response += kernel_val * texture(_texture, _tex_coord + (vec2(1, 0) * step_size));

    kernel_sum += kernel_val;

    kernel_val = _kernel[3];
    if (_vertical)
        response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 2) * step_size));
    else
        response += kernel_val * texture(_texture, _tex_coord + (vec2(2, 0) * step_size));

    kernel_sum += kernel_val;

    if (kernel_sum != 0)
        response /= kernel_sum;

    gl_FragColor = response;
}
