// Copyright 2021 Clemens Cords (mail@clemens-cords.com)
// generated by /crisp/gpu_side/.code_generation/convolute_matmxn.lua on 09.11.2021, 17:24:31

#version 330 core

in vec2 _tex_coord;

uniform mat2x3 _kernel;
uniform sampler2D _texture;

void main()
{
    vec2 step_size = 1 / textureSize(_texture, 0);

    float kernel_sum = 0;
    vec4 response = vec4(0);
    float kernel_val;

    kernel_val = _kernel[0][0];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, -1) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[0][1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, 0) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[0][2];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, 1) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[1][0];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, -1) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[1][1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 0) * step_size));
    kernel_sum += kernel_val;

    kernel_val = _kernel[1][2];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 1) * step_size));
    kernel_sum += kernel_val;

    if (kernel_sum != 0)
        response /= kernel_sum;

    gl_FragColor = response;
}