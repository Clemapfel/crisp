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

using namespace crisp;

const char *vertexShaderSource = R"(

    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 ourColor;
    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor;
        TexCoord = aTexCoord;
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
      
    in vec3 ourColor;
    in vec2 TexCoord;
    
    uniform sampler2D ourTexture;
    
    void main()
    {
        FragColor = vec4(texture(ourTexture, TexCoord).xxx, 1);
    }
)";

int main()
{
    crisp::ColorImage image = crisp::load_color_image("/home/clem/Workspace/crisp/.test/opal_color.png");

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 0;

    auto style = sf::Style::Titlebar | sf::Style::Close;

    auto window = sf::RenderWindow();
    window.create(sf::VideoMode(image.get_size().x(), image.get_size().y()), "", style, context_settings);
    window.setActive(true);

    // shaders
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
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
        std::cerr << log << std::endl;
        return 1;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (not success)
    {
        glGetProgramInfoLog(fragment_shader, sizeof(log), nullptr, log);
        std::cerr << log << std::endl;
        return 1;
    }

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (not success)
    {
        glGetProgramInfoLog(shader_program, sizeof(log), nullptr, log);
        std::cerr << log << std::endl;
        return 1;
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
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::vector<float> data;

    for (size_t y = 0; y < image.get_size().y(); ++y)
    {
        for (size_t x = 0; x < image.get_size().x(); ++x)
        {
            auto px = image(x, image.get_size().y() - y);
            data.push_back(px.red());
            //data.push_back(px.green());
            //data.push_back(px.blue());
        }
    }

    // GL_R3_G3_B2 for black and white
    // GL_RGB32F for color
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, image.get_size().x(), image.get_size().y(), 0, GL_RED, GL_FLOAT, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

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

