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
#include <SFML/Graphics.hpp>

#include <system.hpp>
#include <GLES3/gl3.h>

#include <gpu_side/shader.hpp>

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

    std::string path = "/home/clem/Workspace/crisp/include/gpu_side/.shaders/median_filter_5x5.glsl";
    auto shader = Shader(path);

    shader.set_texture("_texture", image);
    shader.set_vec2("_texture_size", image.get_size());

    shader.set_active();
    shader.bind_uniforms();

    //https://gamedev.stackexchange.com/questions/31162/updating-texture-memory-via-shader
    //http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

    // render
    State::display();
    window.display();

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space)
            {
                shader.load_from_file(path);
                shader.set_active();
                shader.bind_uniforms();
                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

