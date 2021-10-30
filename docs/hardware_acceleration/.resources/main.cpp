// 
// Copyright 2021 Clemens Cords
// Created on 30.10.21 by clem (mail@clemens-cords.com)
//

#include <system/image_io.hpp>
#include <benchmark.hpp>

#include <gpu_side/texture.hpp>

using namespace crisp;

int main()
{
    auto image = load_color_image("/home/clem/Workspace/crisp/docs/hardware_acceleration/.resources/color_opal.png");
    auto texture = Texture<float, 3>(image);

    auto copy_benchmark = Benchmark([&](){
        volatile auto deep_copy = texture;
    });

    std::cout << copy_benchmark.execute(1000) << std::endl;

}

