// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#include <system/input_handler.hpp>

namespace crisp
{
    inline RenderWindow::RenderWindow()
        : sf::RenderWindow()
    {
        InputHandler::set_window(this);
    }

    inline RenderWindow::RenderWindow(size_t width, size_t height)
    {
        InputHandler::set_window(this);
        create(width, height);
    }

    inline void RenderWindow::create(size_t width, size_t height, bool fullscreen, size_t fps_limit)
    {
        _resolution[0] = width;
        _resolution[1] = height;

        sf::ContextSettings context_settings;
        context_settings.antialiasingLevel = 0;

        auto style = sf::Style::Titlebar | sf::Style::Close;

        if (fullscreen)
            style |= sf::Style::Fullscreen;

        sf::RenderWindow::create(sf::VideoMode(width, height), "", style, context_settings);
        setFramerateLimit(fps_limit);

        _background_shape.setPosition(0, 0);
        _background_shape.setSize(sf::Vector2f{width, height});
    }

    inline bool RenderWindow::is_open() const
    {
        return sf::RenderWindow::isOpen();
    }

    inline Vector2ui RenderWindow::get_resolution() const
    {
        size_t x = _resolution.at(0);
        size_t y = _resolution.at(1);
        auto out = Vector2ui{x, y};
        return out;
    }

    inline void RenderWindow::set_background_color(RGB color)
    {
        _background_shape.setPosition(0, 0);

        auto to_uint = [](float in) -> uint8_t {return static_cast<uint8_t>(in * uint8_t(255));};

        _background_shape.setFillColor(sf::Color(to_uint(color.red()), to_uint(color.green()), to_uint(color.blue()), 255));
    }

    inline sf::Time RenderWindow::update()
    {
        auto time = _elapsed.restart();
        InputHandler::update(time);
        return time;
    }

    inline void RenderWindow::clear(RGB color)
    {
        sf::RenderWindow::clear(sf::Color(color.red() * 255, color.green() * 255, color.blue() * 255, 255));
        draw(_background_shape);
    }

    inline Vector2f RenderWindow::get_mouse_position()
    {
        auto pos = sf::Mouse::getPosition(*this);
        return Vector2f{float(pos.x), float(pos.y)};
    }
}