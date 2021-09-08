// 
// Copyright 2021 Clemens Cords
// Created on 08.09.21 by clem (mail@clemens-cords.com)
//

#include <color.hpp>

#include <SFML/Graphics/Image.hpp>

#include <iostream>

namespace crisp
{
    ColorImage load_color_image(std::string path)
    {
        sf::Image image;
        if (not image.loadFromFile(path))
        {
            std::cerr << "[WARNING] file " << path << " not found" << std::endl;
            return ColorImage();
        }

        ColorImage out;
        out.create(image.getSize().x, image.getSize().y);

        for (long x = 0; x < image.getSize().x; ++x)
        {
            for (long y = 0; y < image.getSize().y; ++y)
            {
                auto color = image.getPixel(x, y);
                out(x, y) = RGB(color.r / 255.f, color.g / 255.f, color.b / 255.f);
            }
        }

        return out;
    }

    GrayScaleImage load_grayscale_image(std::string path)
    {
        sf::Image image;
        if (not image.loadFromFile(path))
        {
            std::cerr << "[WARNING] file " << path << " not found" << std::endl;
            return GrayScaleImage();
        }

        GrayScaleImage out;
        out.create(image.getSize().x, image.getSize().y);

        for (long x = 0; x < image.getSize().x; ++x)
        {
            for (long y = 0; y < image.getSize().y; ++y)
            {
                auto color = image.getPixel(x, y);
                out(x, y) = (color.r / 255.f + color.g / 255.f + color.b / 255.f) / 3.f;
            }
        }

        return out;
    }

    BinaryImage load_binary_image(std::string path)
    {
        sf::Image image;
        if (not image.loadFromFile(path))
        {
            std::cerr << "[WARNING] file " << path << " not found" << std::endl;
            return BinaryImage();
        }

        BinaryImage out;
        out.create(image.getSize().x, image.getSize().y);

        for (long x = 0; x < image.getSize().x; ++x)
        {
            for (long y = 0; y < image.getSize().y; ++y)
            {
                auto color = image.getPixel(x, y);

                if ((color.r + color.g + color.b) == 0)
                    out(x, y) = false;
                else if ((color.r == 255 and color.g == 255 and color.b == 255))
                    out(x, y) = true;
                else
                {
                    std::cerr << "[WARNING] loading binary image from an image file containing more than just black (rgb(0, 0, 0)) and white (rgb(1, 1, 1)) pixels. A threshold of 0.5 will be applied." << std::endl;
                    out(x, y) = ((color.r / 255.f + color.g / 255.f + color.b / 255.f) / 3.f) > 0.5;
                }
            }
        }

        return out;
    }

    bool save_to_disk(const ColorImage& in, std::string path)
    {
        sf::Image image;
        image.create(in.get_size().x(), in.get_size().y());

        for (long x = 0; x < in.get_size().x(); ++x)
        {
            for (long y = 0; y < in.get_size().y(); ++y)
            {
                RGB color_in = in(x, y);
                image.setPixel(x, y, sf::Color(uint8_t(color_in.red() * 255),
                                               uint8_t(color_in.green() * 255),
                                               uint8_t(color_in.blue() * 255),
                                               uint8_t(255)));
            }
        }

        if (not image.saveToFile(path))
        {
            std::cerr << "[WARNING] could not save to file " << path << std::endl;
            return false;
        }

        return true;
    }

    bool save_to_disk(const GrayScaleImage & in, std::string path)
    {
        sf::Image image;
        image.create(in.get_size().x(), in.get_size().y());

        for (long x = 0; x < in.get_size().x(); ++x)
        {
            for (long y = 0; y < in.get_size().y(); ++y)
            {
                float intensity = in(x, y);
                image.setPixel(x, y, sf::Color(uint8_t(intensity * 255),
                                               uint8_t(intensity * 255),
                                               uint8_t(intensity * 255),
                                               uint8_t(255)));
            }
        }

        if (not image.saveToFile(path))
        {
            std::cerr << "[WARNING] could not save to file " << path << std::endl;
            return false;
        }

        return true;
    }

    bool save_to_disk(const BinaryImage & in, std::string path)
    {
        sf::Image image;
        image.create(in.get_size().x(), in.get_size().y());

        for (long x = 0; x < in.get_size().x(); ++x)
        {
            for (long y = 0; y < in.get_size().y(); ++y)
            {
                bool which = in(x, y);

                if (which)
                    image.setPixel(x, y, sf::Color::White);
                else
                    image.setPixel(x, y, sf::Color::Black);
            }
        }

        if (not image.saveToFile(path))
        {
            std::cerr << "[WARNING] could not save to file " << path << std::endl;
            return false;
        }

        return true;
    }
}