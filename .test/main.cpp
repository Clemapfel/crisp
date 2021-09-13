// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <image.hpp>
#include <color.hpp>
#include <image.hpp>
#include <system/image_io.hpp>
#include <system/sprite.hpp>
#include <system/render_window.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>
#include <pseudocolor_mapping.hpp>
#include <fourier_transform.hpp>
#include <edge_detection.hpp>
#include <segmentation.hpp>
#include <whole_image_transform.hpp>

#include <noise_generator.hpp>


#include <iostream>

using namespace crisp;

int main()
{
    // load
    auto image = load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    GrayScaleImage plane = image.get_nths_plane<1>();

    // corrupt
    auto noise = SaltAndPepperNoise(0, 0.05);
    for (auto& px : plane)
        px = float(px) + 0.2f + noise(); //std::max<float>(px + 0.2f - noise(), 1.f);

        /*
    image.set_nths_plane<1>(plane);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/image/opal_corrupted.png");

    // investigate planes
    plane = (image.get_nths_plane<0>());
    save_to_disk(plane, "/home/clem/Workspace/crisp/docs/image/opal_corrupted_0.png");

    plane = (image.get_nths_plane<2>());
    save_to_disk(plane, "/home/clem/Workspace/crisp/docs/image/opal_corrupted_2.png");
*/
    plane = (image.get_nths_plane<1>());
    save_to_disk(plane, "/home/clem/Workspace/crisp/docs/image/opal_corrupted_1.png");

    // restore
    auto median_filter = SpatialFilter();
    median_filter.set_kernel(median_filter.box(3, 1));
    median_filter.set_evaluation_function(SpatialFilter::MEDIAN);
    median_filter.apply_to(plane);
    normalize(plane);

    /*
    save_to_disk(plane, "/home/clem/Workspace/crisp/docs/image/opal_restored_1.png");

    image.set_nths_plane<1>(plane);
    save_to_disk(image, "/home/clem/Workspace/crisp/docs/image/opal_restored_2.png");
*/
    auto plan_backup = plane;
    auto sprite = Sprite();
    sprite.set_scale(2);

    float value = 0.075f;
    auto update = [&]()
    {
        plane = plan_backup;
        std::cout << value << std::endl;

        for (auto& px : plane)
            px = crisp::clamp<float>(0.f, 1.f, px - value);

        image.set_nths_plane<1>(plane);
        save_to_disk(image, "/home/clem/Workspace/crisp/docs/image/opal_restored_3.png");

        sprite.create_from(image);
    };

    update();

    auto size = sprite.get_size();
    auto window = RenderWindow(sprite.get_size().x(), sprite.get_size().y());

    while (window.is_open())
    {
        auto time = window.update();

        if (InputHandler::was_key_pressed(UP))
        {
            value += 0.005;
            update();
        }

        if (InputHandler::was_key_pressed(DOWN))
        {
            value -= 0.005;
            update();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}

