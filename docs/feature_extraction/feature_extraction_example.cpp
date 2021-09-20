// 
// Copyright 2021 Clemens Cords
// Created on 20.09.21 by clem (mail@clemens-cords.com)
//

#include <system/image_io.hpp>
#include <image/grayscale_image.hpp>
#include <segmentation.hpp>
#include <image_region.hpp>

using namespace crisp;

int main()
{
    auto image = load_color_image("/home/clem/Workspace/crisp/docs/feature_extraction/pepper.png");
    auto as_grayscale = image.get_value_plane();

    auto thresholded = manual_threshold(as_grayscale, 0.01f);
    auto segments = decompose_into_connected_segments(thresholded);
    auto pepper_segment = segments.at(1);
    auto pepper = ImageRegion(pepper_segment, image);

    for (auto& px : image)
        px *= 0.5;

    for (auto& px : pepper.get_boundary())
        image(px.x(), px.y()) = RGB{0.9, 0, 0.9};

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/feature_extraction/pepper_boundary.png");
}

