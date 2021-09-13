// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace crisp
{
    inline Sprite::Sprite()
    {}

    inline Sprite::Sprite(Vector2f position)
    {
        set_position(position);
    }

    inline void Sprite::set_position(Vector2f pos)
    {
        _position.x = pos.x();
        _position.y = pos.y();
    }

    inline Vector2f Sprite::get_position() const
    {
        return Vector2f{_position.x, _position.y};
    }

    inline Vector2f Sprite::get_size() const
    {
        auto size = _sprite.getTexture()->getSize();
        return Vector2f{float(size.x * _scale), float(size.y * _scale)};
    }

    inline size_t Sprite::get_scale() const
    {
        return _scale;
    }

    void Sprite::set_scale(size_t scale)
    {
        _scale = scale;
        _sprite.setScale(scale, scale);
    }

    void Sprite::update()
    {
        _sprite.setTextureRect(sf::IntRect(0, 0, _texture.getSize().x, _texture.getSize().y));
        _sprite.setTexture(_texture);
        _sprite.setOrigin(_sprite.getPosition().x * 0.5, _sprite.getPosition().y * 0.5);
        _sprite.setScale(_scale, _scale);
        _sprite.setPosition(_position);
    }

    template<typename T>
    inline void Sprite::create_from(const Image<T, 1>& image)
    {
        sf::Image temp;
        temp.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                T value = image(x, y);
                auto as_float = static_cast<float>(value);
                temp.setPixel(x, y, sf::Color(as_float * 255, as_float * 255, as_float * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    inline void Sprite::create_from(const BinaryImage& image)
    {
        sf::Image temp;
        temp.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                bool which = image(x, y);

                if (which)
                    temp.setPixel(x, y, sf::Color::White);
                else
                    temp.setPixel(x, y, sf::Color::Black);
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    inline void Sprite::create_from(const GrayScaleImage& image)
    {
        sf::Image temp;
        temp.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                auto intensity = image(x, y);
                temp.setPixel(x, y, sf::Color(intensity * 255.f, intensity * 255.f, intensity * 255.f, 255.f));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    inline void Sprite::create_from(const ColorImage& image)
    {
        sf::Image temp;
        temp.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                auto color = image(x, y);
                temp.setPixel(x, y, sf::Color(color.red() * 255, color.green() * 255, color.blue() * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    template<FourierTransformMode Mode>
    inline void Sprite::create_from(const FourierTransform<Mode>& transform)
    {
        auto image = transform.as_image();

        sf::Image temp;
        temp.create(image.get_size().x(), image.get_size().y());

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                auto value = image(x, y);
                temp.setPixel(x, y, sf::Color(float(value) * 255, float(value) * 255, float(value) * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    inline void Sprite::create_from(const FrequencyDomainFilter& filter)
    {
        sf::Image temp;
        temp.create(filter.get_size().x(), filter.get_size().y());

        for (long x = 0; x < filter.get_size().x(); ++x)
        {
            for (long y = 0; y < filter.get_size().y(); ++y)
            {
                auto value = filter(x, y);
                temp.setPixel(x, y, sf::Color(float(value) * 255, float(value) * 255, float(value) * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    void Sprite::create_from(const Kernel& kernel)
    {
        sf::Image temp;
        temp.create(kernel.rows(), kernel.cols());

        for (long x = 0; x < kernel.rows(); ++x)
        {
            for (long y = 0; y < kernel.cols(); ++y)
            {
                auto value = kernel(x, y);
                temp.setPixel(x, y, sf::Color(float(value) * 255, float(value) * 255, float(value) * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    template<size_t N>
    void Sprite::create_from(const Histogram <N>& histogram)
    {
        sf::Image temp;
        temp.create(N, N, sf::Color::Black);

        float n = 0;
        float mean = 0;
        size_t max = 0;
        for (const auto& pair : histogram)
        {
            max = std::max(max, pair.second);
            mean += pair.first * pair.second;
            n += pair.second;
        }

        mean /= n;

        size_t step = ceil(float(max) / (0.85 * N));

        if (N > 1920)
            std::cerr << "[Warning] rendering a histogram with " << N << " different values to a texture will take up a large amount of memory" << std::endl;

        for (size_t x = 0; x < N; ++x)
        {
            if (x == mean)
                for (size_t y = 0; y < temp.getSize().y; ++y)
                    temp.setPixel(x, y, sf::Color::Red);

            for (size_t y = 1; y * step < histogram.at(x); ++y)
            {
                temp.setPixel(x, temp.getSize().y - y, sf::Color::White);
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    void Sprite::create_from(const StructuringElement & se)
    {
        sf::Image temp;
        temp.create(se.rows(), se.cols());

        for (long x = 0; x < se.rows(); ++x)
        {
            for (long y = 0; y < se.cols(); ++y)
            {
                float value = se(x, y).has_value() ? (se(x, y).value() ? 1.f : 0.5f) : 0.f;
                temp.setPixel(x, y, sf::Color(float(value) * 255, float(value) * 255, float(value) * 255, 255));
            }
        }

        _texture.loadFromImage(temp);
        _position = _sprite.getOrigin();
        update();
    }

    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = &_texture;
        target.draw(_sprite, states);
    }
}