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
#include <GL/gl.h>

using namespace crisp;

int main()
{
    auto settings = sf::ContextSettings(0, 0, 0, 3, 3);
    auto context = sf::Context(settings, 1000, 1000);

    // render texture has context
    auto render = sf::RenderTexture();
    render.create(800, 600, settings);

    render.setActive(true);
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    render.setActive(false);

    auto window = crisp::RenderWindow(800, 600);
    auto sprite = sf::Sprite();
    sprite.setTexture(render.getTexture());

    while(window.is_open())
    {
        window.update();

        window.clear();
        window.draw(sprite);
        window.display();
    }

}

