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
#include <GLES3/gl32.h>

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

    std::string path = "/home/clem/Workspace/crisp/include/gpu_side/.shaders/test.glsl";
    auto change_shader = Shader(path);

    GLNativeHandle buffer_id;
    glGenFramebuffers(1, &buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);

    auto texture_id = change_shader.set_texture("_texture", image);
    change_shader.set_active();
    change_shader.bind_uniforms();

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
    glViewport(0, 0, image.get_size().x(), image.get_size().y());

    State::display();
    window.display();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, image.get_size().x(), image.get_size().y());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glPopAttrib();

    State::bind_framebuffer(-1);

    auto noop_shader = change_shader;
    noop_shader.load_from_file("/home/clem/Workspace/crisp/include/gpu_side/.shaders/noop.frag");
    noop_shader.set_active();
    noop_shader.bind_uniforms();

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
                State::display();
                window.display();
            }
            else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::B)
            {
                change_shader.load_from_file(path);
                change_shader.set_active();
                change_shader.bind_uniforms();
                State::display();
                window.display();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

