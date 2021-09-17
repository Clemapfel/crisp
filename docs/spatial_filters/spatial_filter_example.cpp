// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>
#include <system/image_io.hpp>
#include <iostream>
#include <whole_image_transform.hpp>

using namespace crisp;

int main()
{
    auto original = load_grayscale_image("/home/clem/Workspace/crisp/docs/spatial_filters/color_opal.png");
    auto image = original;

    auto filter = SpatialFilter();
    filter.set_evaluation_function(filter.CONVOLUTION);
    Kernel kernel;

    kernel = SpatialFilter::identity(3);
    std::cout << "// identity\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/identity.png");

    kernel = SpatialFilter::one(3);
    std::cout << "// one\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    normalize(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/one.png");

    kernel = SpatialFilter::zero(3);
    std::cout << "// zero\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/zero.png");

    kernel = SpatialFilter::box(3, 0.5f);
    std::cout << "// box\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    normalize(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/box.png");

    kernel = SpatialFilter::normalized_box(3);
    std::cout << "// normalized_box\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    normalize(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/normalized_box.png");

    kernel = SpatialFilter::gaussian(5);
    std::cout << "// gaussian\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/gaussian.png");

    kernel = SpatialFilter::laplacian_first_derivative();
    std::cout << "// laplace 1\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/laplacian_first.png");

    kernel = SpatialFilter::laplacian_second_derivative();
    std::cout << "// laplace 2\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/laplacian_second.png");

    kernel = SpatialFilter::laplacian_of_gaussian(5);
    std::cout << "// log\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/log.png");

    kernel = SpatialFilter::simple_gradient_x();
    std::cout << "// simple gradient x\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/simple_gradient_x.png");

    kernel = SpatialFilter::simple_gradient_y();
    std::cout << "// simple gradient y\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/simple_gradient_y.png");

    kernel = SpatialFilter::roberts_gradient_x();
    std::cout << "// roberts x\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/roberts_x.png");

    kernel = SpatialFilter::prewitt_gradient_x();
    std::cout << "// prewitt x\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/prewitt_x.png");

    kernel = SpatialFilter::sobel_gradient_x();
    std::cout << "// sobel x\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/sobel_x.png");

    kernel = SpatialFilter::kirsch_compass_n();
    std::cout << "// kirsch n\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/kirsch_n.png");

    kernel = SpatialFilter::kirsch_compass_e();
    std::cout << "// kirsch e\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/kirsch_e.png");

    kernel = SpatialFilter::kirsch_compass_ne();
    std::cout << "// kirsch ne\n" << kernel << "\n" << std::endl;
    image = original;
    filter.set_kernel(kernel);
    filter.apply_to(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/spatial_filters/kirsch_ne.png");


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

    Kernel k1 = SpatialFilter::laplacian_first_derivative();
    std::cout << k1 << "\n" << std::endl;

    Kernel res = convolute(k1, k1);
    std::cout << res << "\n" << std::endl;
     */
}

