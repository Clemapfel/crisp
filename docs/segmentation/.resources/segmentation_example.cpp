// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <segmentation.hpp>
#include <system/image_io.hpp>
#include <image/binary_image.hpp>
#include <image/grayscale_image.hpp>
#include <edge_detection.hpp>
using namespace crisp;

int main()
{
    auto clean = load_grayscale_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/opal_clean.png");
    auto non_uniform = load_grayscale_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/opal_non_uniform.png");

    save_to_disk(clean, "/home/clem/Workspace/crisp/docs/segmentation/.resources/uniform.png");
    save_to_disk(non_uniform, "/home/clem/Workspace/crisp/docs/segmentation/.resources/non_uniform.png");
    
    auto binary = Segmentation::manual_threshold(clean, 0.5f);

    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/manual_01.png");
    binary = Segmentation::manual_threshold(non_uniform, 0.5f);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/manual_02.png");

    binary = Segmentation::basic_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/basic_01.png");
    binary = Segmentation::basic_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/basic_02.png");

    binary = Segmentation::otsu_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/otsu_01.png");
    binary = Segmentation::otsu_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/otsu_02.png");

    binary = Segmentation::variable_threshold(clean, 0.5);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/variable_01.png");
    binary = Segmentation::variable_threshold(non_uniform, 0.1);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/variable_02.png");

    binary = Segmentation::neighborhood_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/neighborhood_01.png");
    binary = Segmentation::neighborhood_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/.resources/neighborhood_02.png");

    auto gradient = EdgeDetection::threshold_gradient(clean);
    gradient.invert();
    save_to_disk(gradient, "/home/clem/Workspace/crisp/docs/segmentation/.resources/edge_sobel.png");

    gradient = EdgeDetection::canny(clean);
    save_to_disk(gradient, "/home/clem/Workspace/crisp/docs/segmentation/.resources/edge_canny.png");

    return 0;

    /*
    auto image = load_color_image("/home/clem/Workspace/crisp/docs/segmentation/.resources/rainbow_lorikeet.jpg");
    
    // k-means
    auto k_means = Segmentation::k_means_clustering(image, 5);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_inf.png");

    k_means = Segmentation::k_means_clustering(image, 5, 1);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_1.png");

    k_means = Segmentation::k_means_clustering(image, 5, 2);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_2.png");

    k_means = Segmentation::k_means_clustering(image, 5, 3);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_3.png");

    k_means = Segmentation::k_means_clustering(image, 5, 5);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_5.png");

    k_means = Segmentation::k_means_clustering(image, 5, 10);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/.resources/k_means_10.png");

    return 0;

    // superpixel
    auto superpixel = Segmentation::superpixel_clustering(image, 300);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_inf.png");

    superpixel = Segmentation::superpixel_clustering(image, 10, 1);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_1.png");

    superpixel = Segmentation::superpixel_clustering(image, 10, 2);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_2.png");

    superpixel = Segmentation::superpixel_clustering(image, 10, 3);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_3.png");

    superpixel = Segmentation::superpixel_clustering(image, 10, 5);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_5.png");

    superpixel = Segmentation::superpixel_clustering(image, 10, 10);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/.resources/superpixel_10.png");
    return 0;
    */
}

