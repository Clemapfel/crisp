// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <image.hpp>

#include <iostream>

using namespace crisp;

int main()
{
    Scalar<bool> test = true;

    std::cout << (test or false) << std::endl;

    Image<bool> bool_image;

    return 0;
}

