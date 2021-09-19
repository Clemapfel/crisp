// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <segmentation.hpp>
#include <system/image_io.hpp>
#include <image/binary_image.hpp>
#include <image/grayscale_image.hpp>
using namespace crisp;

int main()
{
    auto clean = load_grayscale_image("/home/clem/Workspace/crisp/docs/segmentation/opal_clean.png");
    auto non_uniform = load_grayscale_image("/home/clem/Workspace/crisp/docs/segmentation/opal_non_uniform.png");


    auto binary = crisp::manual_threshold(clean, 0.5f);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/manual_01.png");
    binary = crisp::manual_threshold(non_uniform, 0.5f);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/manual_02.png");

    binary = crisp::basic_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/basic_01.png");
    binary = crisp::basic_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/basic_02.png");

    binary = crisp::otsu_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/otsu_01.png");
    binary = crisp::otsu_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/otsu_02.png");

    binary = crisp::variable_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/variable_01.png");
    binary = crisp::variable_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/variable_02.png");

    binary = crisp::neighborhood_threshold(clean);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/neighborhood_01.png");
    binary = crisp::neighborhood_threshold(non_uniform);
    save_to_disk(binary, "/home/clem/Workspace/crisp/docs/segmentation/neighborhood_02.png");

    auto image = load_color_image("/home/clem/Workspace/crisp/docs/segmentation/rainbow_lorikeet.jpg");

    /*
    // k-means
    auto k_means = crisp::k_means_clustering(image, 10);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_inf.png");

    k_means = crisp::k_means_clustering(image, 10, 1);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_1.png");

    k_means = crisp::k_means_clustering(image, 10, 2);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_2.png");

    k_means = crisp::k_means_clustering(image, 10, 3);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_3.png");

    k_means = crisp::k_means_clustering(image, 10, 5);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_5.png");

    k_means = crisp::k_means_clustering(image, 10, 10);
    save_to_disk(k_means, "/home/clem/Workspace/crisp/docs/segmentation/k_means_10.png");
*/
    // superpixel
    auto superpixel = crisp::superpixel_clustering(image, 300);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_inf.png");

    superpixel = crisp::superpixel_clustering(image, 10, 1);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_1.png");

    superpixel = crisp::superpixel_clustering(image, 10, 2);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_2.png");

    superpixel = crisp::superpixel_clustering(image, 10, 3);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_3.png");

    superpixel = crisp::superpixel_clustering(image, 10, 5);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_5.png");

    superpixel = crisp::superpixel_clustering(image, 10, 10);
    save_to_disk(superpixel, "/home/clem/Workspace/crisp/docs/segmentation/superpixel_10.png");



    /*
    auto image = load_binary_image("/home/clem/Workspace/crisp/docs/segmentation/blobs.png");
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/segmentation/blobs.png");

    float saturation = 1;
    float value = 1;

    ColorImage segmented;
    segmented.create(image.get_size().x(), image.get_size().y());

    auto segments_unconnected = crisp::decompose_into_segments(image);

    float step = 1.f / segments_unconnected.size();
    size_t i = 0;

    for (auto& segment : segments_unconnected)
    {
        for (auto& px : segment)
            segmented(px.x(), px.y()) = HSV{step * i, saturation, value}.to_rgb();

        i++;
    }

    save_to_disk(segmented, "/home/clem/Workspace/crisp/docs/segmentation/unconnected_segments.png");

    auto segments_connected = crisp::decompose_into_connected_segments(image);

    step = 1.f / segments_connected.size();
    i = 0;

    for (auto& segment : segments_connected)
    {
        for (auto& px : segment)
            segmented(px.x(), px.y()) = HSV{step * i, saturation, value}.to_rgb();

        i++;
    }

    save_to_disk(segmented, "/home/clem/Workspace/crisp/docs/segmentation/connected_segments.png");
    */

    return 0;
}

