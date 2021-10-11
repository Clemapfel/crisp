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

#include <texture/state.hpp>
#include <texture/texture.hpp>

using namespace crisp;

int main()
{
    auto image = crisp::load_binary_image("/home/clem/Workspace/crisp/.test/opal_color.png");
    //auto image = image_in.convert_to_color();

    for (size_t y = 0; y < image.get_size().y(); ++y)
        image(0, y) = false;

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto state = crisp::State<sf::RenderWindow>();
    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x(), image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    // shaders
    std::ifstream file("/home/clem/Workspace/crisp/.test/default.vert");
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto vertex_source_str = buffer.str();
    const auto* vertex_source = vertex_source_str.c_str();

    file.close();
    file.open("/home/clem/Workspace/crisp/.test/default.frag");
    buffer.str(std::string());
    buffer << file.rdbuf();
    auto fragment_source_str = buffer.str();
    const auto* fragment_source = fragment_source_str.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char log[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (not success)
    {
        glGetProgramInfoLog(vertex_shader, sizeof(log), nullptr, log);
        std::cout << log << std::endl;
        throw std::invalid_argument("error compiling vertex shader");
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (not success)
    {
        glGetProgramInfoLog(fragment_shader, sizeof(log), nullptr, log);
        std::cout << log << std::endl;
        throw std::invalid_argument("error compiling fragment shader");
    }

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (not success)
    {
        glGetProgramInfoLog(shader_program, sizeof(log), nullptr, log);
        std::cout << log << std::endl;
        throw std::invalid_argument("error linking shaders");
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {
    //  pos         colors      tex
         1,  1, 0,   1, 1, 1,   1, 1, // top right
         1, -1, 0,   1, 1, 1,   1, 0, // bottom right
        -1, -1, 0,   1, 1, 1,   0, 0, // bottom left
        -1,  1, 0,   1, 1, 1,   0, 1  // top left
    };

    unsigned int indices[] =
    {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // vertices
    unsigned int vertex_buffer, vertex_array, element_array_buffer;
    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_array_buffer);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    auto tex = Texture<bool, 1>(image.get_size().x(), image.get_size().y());
    tex.create_from(image);
    auto texture = state.add_texture(tex);
    state.bind_texture(texture);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    window.display();

    while(window.isOpen())
    {
        auto event = sf::Event();
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::EventType::Closed)
                window.close();
        }
    }
}

