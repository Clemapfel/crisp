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

    auto major = pepper.get_major_axis();
    auto minor = pepper.get_minor_axis();

    for (auto& px : image)
        px *= 0.5;

    image(major.first.x(), major.first.y()) = RGB{1, 0, 1};
    image(major.second.x(), major.second.y()) = RGB{1, 0, 1};

    image(minor.first.x(), minor.first.y()) = RGB{0, 1, 0};
    image(minor.second.x(), minor.second.y()) = RGB{0, 1, 0};

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/feature_extraction/pepper_eigenvectors.png");
    return 0;

    /*
    size_t bad_n = 0;
    for (auto& px : pepper_segment)
    {
        size_t n_unconnected = 0;
        for (int i = -1; i <= +1; ++i)
        {
            for (int j = -1; j <= +1; ++j)
            {
                // outer edge of image is always boundary
                if (px.x() + i < 0 or px.x() + i > image.get_size().x() or px.y() + j < 0 or
                    px.y() + j > image.get_size().y())
                    n_unconnected++;

                else if (not(i == 0 and j == 0) and pepper_segment.find(Vector2ui{px.x() + i, px.y() + j}) == pepper_segment.end())
                    n_unconnected++;

                if (n_unconnected > 0)
                {
                    image(px.x(), px.y()) = RGB{0, 1, 1};
                    bad_n++;
                }
            }
        }
    }
     */

    for (auto& px : pepper.get_boundary())
        image(px.x(), px.y()) = RGB{1, 0, 1};

    auto rect = pepper.get_axis_aligned_bounding_box();

    auto y = rect.at(0).y();
    for (size_t x = rect.at(0).x(); x <= rect.at(1).x(); ++x)
        image(x, y) = RGB{1, 1, 1};

    auto x = rect.at(1).x();
    for (size_t y = rect.at(1).y(); y <= rect.at(2).y(); ++y)
        image(x, y) = RGB{1, 1, 1};

    y = rect.at(2).y();
    for (size_t x = rect.at(3).x(); x <= rect.at(2).x(); ++x)
        image(x, y) = RGB{1, 1, 1};

    x = rect.at(0).x();
    for (size_t y = rect.at(0).y(); y <= rect.at(3).y(); ++y)
        image(x, y) = RGB{1, 1, 1};

    save_to_disk(image, "/home/clem/Workspace/crisp/docs/feature_extraction/pepper_aabb.png");
}

