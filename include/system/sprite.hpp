// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/binary_image.hpp>
#include <image/color_image.hpp>
#include <image/multi_plane_image.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace crisp
{
    class Sprite : public sf::Drawable
    {
        public:
            Sprite();
            Sprite(Vector2f position);

            // @brief set center of sprite
            void set_position(Vector2f);
            Vector2f get_position() const;

            Vector2f get_size() const;

            void set_scale(size_t);
            size_t get_scale() const;

            void create_from(const BinaryImage&);
            void create_from(const GrayScaleImage&);
            void create_from(const ColorImage&);

        private:
            void update();

            sf::Vector2f _position;
            size_t _scale = 1;

            sf::Texture _texture;
            sf::Sprite _sprite;
    };
}

#include ".src/sprite.inl"