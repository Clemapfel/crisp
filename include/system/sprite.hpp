// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/binary_image.hpp>
#include <image/color_image.hpp>
#include <image/multi_plane_image.hpp>
#include <fourier_transform.hpp>
#include <histogram.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>
#include <frequency_domain_filter.hpp>

namespace crisp
{
    // class that allows for rendering of almost all crisp objects
    class Sprite : public sf::Drawable
    {
        public:
            // @brief default ctor
            Sprite();

            // @brief ctor and assign position
            // @param : position
            Sprite(Vector2f);

            // @brief set center of sprite
            // @param : center
            void set_position(Vector2f);

            // @brief get center of sprite
            // @returns center as vector
            Vector2f get_position() const;

            // @brief get size including scale
            // @returns size as vector
            Vector2f get_size() const;

            // @brief set scale
            // @param : unsigned integer
            void set_scale(size_t);

            // @brief get scale
            // @returns scale as unsignend integer
            size_t get_scale() const;

            // @brief load from binary image
            void create_from(const BinaryImage&);

            // @brief load from grayscale image
            void create_from(const GrayScaleImage&);

            // @brief load from color image
            void create_from(const ColorImage&);

            // @brief load from fourier spectrum
            template<FourierTransformMode Mode>
            void create_from(const FourierTransform<Mode>&);

            // @brief load from frequency domain filter
            void create_from(const FrequencyDomainFilter&);

            // @brief load from filter kernel
            void create_from(const Kernel&);

            // @brief load from structuring element, "don't care" are black, background are grey (rgb(0.5, 0.5, 0.5)), foreground white
            void create_from(const StructuringElement&);

            // @brief load from histogram
            template<size_t N>
            void create_from(const Histogram<N>&);

        protected:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        private:
            void update();

            sf::Vector2f _position;
            size_t _scale = 1;

            sf::Texture _texture;
            sf::Sprite _sprite;
    };
}

#include ".src/sprite.inl"