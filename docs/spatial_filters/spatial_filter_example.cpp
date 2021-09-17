// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>
#include <system/image_io.hpp>
#include <iostream>

using namespace crisp;

int main()
{
    auto image = load_color_image("/home/clem/Workspace/crisp/docs/spatial_filters/color_opal.png");

    auto kernel = Kernel();
    kernel.resize(5, 5);
    kernel.setConstant(1);  // 5x5 kernel where all values are 1

    auto filter = SpatialFilter();
    filter.set_kernel(kernel);
    filter.set_evaluation_function(filter.CONVOLUTION);

    filter.apply_to(image);

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/opal_initial_filter_color.png");
    return 0;





    /*
    kernel.resize(3, 3);
    kernel << 1, 1, 1,
              1, 1, 1,
              1, 1, 1;

    normalize(kernel);

    std::cout << kernel << std::endl;

    float sum_of_elements = 0;
    for (size_t x = 0; x < kernel.rows(); ++x)
    for (size_t y = 0; y < kernel.cols(); ++y)
        sum_of_elements += kernel(x, y);

    std::cout << "sum after normalization: " << sum_of_elements << std::endl;

    kernel << 1, 2, 3,
              4, 5, 6,
              7, 8, 9;

    for (size_t i = 1; i <= 4; ++i)
    {
        rotate(kernel, 1);
        std::cout << kernel << "\n" << std::endl;
    }

    kernel = SpatialFilter::sobel_gradient_x();

    std::cout << kernel << std::endl;

    Kernel left, right;
    if (not seperate(kernel, &left, &right))
    {
        // handle non-seperable kernels
    }

    std::cout << left << "\n" << std::endl;
    std::cout << right << "\n" << std::endl;

    kernel = left * right;

    std::cout << kernel << std::endl;
    */

    Kernel k1 = SpatialFilter::laplacian_first_derivative();
    std::cout << k1 << "\n" << std::endl;

    Kernel res = convolute(k1, k1);
    std::cout << res << "\n" << std::endl;
}

