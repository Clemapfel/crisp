// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <color.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace crisp
{
    using Drawable = sf::Drawable;
    using Time = sf::Time;

    class RenderWindow : protected sf::RenderWindow
    {
            friend class InputHandler;

        public:
            RenderWindow();
            RenderWindow(size_t width, size_t height);

            void create(size_t width, size_t height, bool fullscreen = false, size_t fps_limit = 60);

            bool is_open() const;
            using sf::RenderWindow::close;

            Vector2ui get_resolution() const;
            void set_background_color(RGB);

            Time update();

            using sf::RenderTarget::draw;
            void clear(RGB = RGB(0, 0, 0));
            using sf::RenderWindow::display;

        protected:
            using sf::RenderWindow::pollEvent;
            Vector2f get_mouse_position();

        private:
            sf::Clock _elapsed;
            Vector2ui _resolution = Vector2ui(0, 0);
            sf::RectangleShape _background_shape;
    };
}

#include ".src/render_window.inl"