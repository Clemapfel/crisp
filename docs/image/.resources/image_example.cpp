// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <system.hpp>
#include <image/grayscale_image.hpp>
using namespace crisp;

int main()
{

    const auto image = load_binary_image("/home/clem/Workspace/crisp/docs/image/color_opal.png");
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/imagebinary_opal.png");

    return 0;
}

