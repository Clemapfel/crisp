// 
// Copyright 2021 Clemens Cords
// Created on 14.09.21 by clem (mail@clemens-cords.com)
//

#include <system.hpp>
#include <morphological_transform.hpp>

using namespace crisp;
int main()
{
    {
        const auto bckp = load_binary_image("/home/clem/Workspace/crisp/docs/morphological_transform/.resources/hmt_template.png");
        auto binary_template = bckp;

        auto transform = MorphologicalTransform();

        auto generate_cross = [](size_t dimensions) -> StructuringElement
        {
            auto out = StructuringElement();
            out.resize(dimensions, dimensions);
            out.setConstant(0);

            for (size_t i = 0; i < dimensions; ++i)
            {
                out(i, dimensions/2) = 1;
                out(dimensions/2, i) = 1;
            }

            return out;
        };

        size_t cross_size = 11;
        transform.set_structuring_element(generate_cross(cross_size));

        transform.hit_or_miss_transform(binary_template);
        save_to_disk(binary_template, "/home/clem/Workspace/crisp/docs/morphological_transform/.resources/hmt.png");

        StructuringElement replace = MorphologicalTransform::all_background(cross_size, cross_size);

        binary_template = bckp;
        transform.replace_pattern(binary_template, replace);
        save_to_disk(binary_template, "/home/clem/Workspace/crisp/docs/morphological_transform/.resources/pattern_replace.png");

        return 0;
    }

    const auto binary = load_binary_image("/home/clem/Workspace/crisp/docs/morphological_transform/binary_template.png");
    const auto grayscale = load_grayscale_image("/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_template.png");
    const auto binary_mask = load_binary_image("/home/clem/Workspace/crisp/docs/morphological_transform/mask.png");
    const auto grayscale_mask = load_grayscale_image("/home/clem/Workspace/crisp/docs/morphological_transform/mask.png");

    auto transform = MorphologicalTransform();
    transform.set_structuring_element(transform.circle(9));

    BinaryImage binary_result = binary;
    GrayScaleImage grayscale_result = grayscale;

    transform.erode(binary_result);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_erode.png");
    transform.erode(grayscale_result);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_erode.png");

    binary_result = binary;
    grayscale_result = grayscale;


    transform.erode(binary_result, binary_mask);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_erode_mask.png");
    transform.erode(grayscale_result, grayscale_mask);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_erode_mask.png");

    binary_result = binary;
    grayscale_result = grayscale;

    transform.dilate(binary_result);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_dilate.png");
    transform.dilate(grayscale_result);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_dilate.png");

    binary_result = binary;
    grayscale_result = grayscale;


    transform.dilate(binary_result, binary_mask);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_dilate_mask.png");
    transform.dilate(grayscale_result, grayscale_mask);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_dilate_mask.png");

    binary_result = binary;
    grayscale_result = grayscale;

    transform.open(binary_result);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_open.png");
    transform.open(grayscale_result);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_open.png");

    binary_result = binary;
    grayscale_result = grayscale;

    transform.close(binary_result);
    save_to_disk(binary_result, "/home/clem/Workspace/crisp/docs/morphological_transform/binary_close.png");
    transform.close(grayscale_result);
    save_to_disk(grayscale_result, "/home/clem/Workspace/crisp/docs/morphological_transform/grayscale_close.png");

    return 0;
}

