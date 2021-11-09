// Copyright 2021 Clemens Cords (mail@clemens-cords.com)

#version 330 core

float project(float lower_bound, float upper_bound, float value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

float to_grayscale(vec4 rgba)
{
    return (rgba.r + rgba.g + rgba.b) / 3;
}

in vec2 _tex_coord;
out vec4 _out;

uniform sampler2D _texture;

void main()
{
    vec2 step_size = 1.f / textureSize(_texture, 0);

    const mat3 sobel_x = mat3(1, 0, -1,
                              2, 0, -2,
                              1, 0, -1);

    const mat3 sobel_y = mat3( 1,  2,  1,
                               0,  0,  0,
                              -1, -2, -1);

    mat3 img;

    img[0][0] = to_grayscale(texture(_texture, _tex_coord + vec2(-1, -1) * step_size));
    img[0][1] = to_grayscale(texture(_texture, _tex_coord + vec2(-1, 0) * step_size));
    img[0][2] = to_grayscale(texture(_texture, _tex_coord + vec2(-1, 1) * step_size));
    img[1][0] = to_grayscale(texture(_texture, _tex_coord + vec2(0, -1) * step_size));
    img[1][1] = to_grayscale(texture(_texture, _tex_coord + vec2(0, 0) * step_size));
    img[1][2] = to_grayscale(texture(_texture, _tex_coord + vec2(0, 1) * step_size));
    img[2][0] = to_grayscale(texture(_texture, _tex_coord + vec2(1, -1) * step_size));
    img[2][1] = to_grayscale(texture(_texture, _tex_coord + vec2(1, 0) * step_size));
    img[2][2] = to_grayscale(texture(_texture, _tex_coord + vec2(1, 1) * step_size));

    float gx = 0;

    // skipping kernel entries 0 is performance equivalent to convolution with the seperated kernel
    gx += sobel_x[0][0] * img[0][0];
    gx += sobel_x[0][1] * img[0][1];
    gx += sobel_x[0][2] * img[0][2];
    //gx += sobel_x[1][0] * img[1][0];
    //gx += sobel_x[1][1] * img[1][1];
    //gx += sobel_x[1][2] * img[1][2];
    gx += sobel_x[2][0] * img[2][0];
    gx += sobel_x[2][1] * img[2][1];
    gx += sobel_x[2][2] * img[2][2];

    float gy = 0;

    gy += sobel_y[0][0] * img[0][0];
    //gy += sobel_y[0][1] * img[0][1];
    gy += sobel_y[0][2] * img[0][2];
    gx += sobel_y[1][0] * img[1][0];
    //gx += sobel_y[1][1] * img[1][1];
    gx += sobel_y[1][2] * img[1][2];
    gy += sobel_y[2][0] * img[2][0];
    //gy += sobel_y[2][1] * img[2][1];
    gy += sobel_y[2][2] * img[2][2];

    _out = vec4(sqrt(gx * gx + gy * gy));
}