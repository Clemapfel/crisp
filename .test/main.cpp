// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <classification/fully_connected_neural_network.hpp>
#include <classification/bayes_classifier.hpp>
#include <thread>
#include <iostream>
#include <fstream>

#include <SFML/OpenGL.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include <system.hpp>
#include <GLES3/gl3.h>

#include <segmentation.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_grayscale_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x()-1, image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    auto shader = Shader("/home/clem/Workspace/crisp/include/texture/.shaders/noop.frag");

    shader.set_texture("_texture", image);
    shader.set_color("_color", RGB{1, 0, 1});

    shader.set_active();
    shader.bind_uniforms();

    // render
    State::display();
    window.display();

    State::get_pixel_buffer(image);
    save_to_disk(image, "/home/clem/Workspace/crisp/.test/screenshot.png");
    return 0;
}

