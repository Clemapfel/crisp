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

    // window that allows for rendering of sprites
    class RenderWindow : protected sf::RenderWindow
    {
        friend class InputHandler;

        public:
            /// @brief default ctor
            RenderWindow();

            /// @brief ctor and create as specified size
            /// @param width: x-resolution
            /// @param height: y-resolution
            RenderWindow(size_t width, size_t height);

            /// @brief create as specified size
            /// @param width: x-resolution
            /// @param height: y-resolution
            /// @param fullscreen: open in fullscreen mode (default: true)
            /// @param fps_limit: (default: 60)
            void create(size_t width, size_t height, bool fullscreen = false, size_t fps_limit = 60);

            /// @brief is the window currently minimized
            /// @returns false if minimized, true otherwise
            bool is_open() const;

            /// @brief close the window
            using sf::RenderWindow::close;

            /// @brief get window resolution
            /// @returns resolution as vector
            Vector2ui get_resolution() const;

            /// @brief set background color
            /// @param color: color as RGB
            void set_background_color(RGB);

            /// @brief get time since last update
            /// @returns time
            Time update();

            /// @brief draw object
            using sf::RenderTarget::draw;

            /// @brief clear window with color
            /// @param color: color as RGB
            void clear(RGB = RGB(0, 0, 0));

            /// @brief push current render state for display
            using sf::RenderWindow::display;

        protected:
            using sf::RenderWindow::pollEvent;
            Vector2f get_mouse_position();

        private:
            sf::Clock _elapsed;
            Vector2ui _resolution = Vector2ui{0, 0};
            sf::RectangleShape _background_shape;
    };
}

#include ".src/render_window.inl"