#version 330 core

in vec2 _tex_coord;

uniform mat3 _kernel;
uniform sampler2D _texture;
uniform vec2 _texture_size;

void main()
{
    mat3 kernel = mat3(1);
    vec2 step_size = 1 / _texture_size;

    float kernel_sum = 0;
    vec4 response = vec4(0);
    
    float kernel_val = kernel[0][0];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, -1) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[0][1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, 0) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[0][2];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(-1, 1) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[1][0];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, -1) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[1][1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 0) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[1][2];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(0, 1) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[2][0];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(1, -1) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[2][1];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(1, 0) * step_size));
    kernel_sum += kernel_val;
    
    kernel_val = kernel[2][2];
    response += kernel_val * texture(_texture, _tex_coord + (vec2(1, 1) * step_size));
    kernel_sum += kernel_val;

    response /= kernel_sum;
    gl_FragColor = response;
}
