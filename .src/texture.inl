// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/texture.hpp>

namespace crisp
{
    template<typename T, size_t N>
    Texture<T, N>::Texture(size_t width, size_t height)
        : _size{width, height}
    {
        create(width, height);
    }

    template<typename T, size_t N>
    void Texture<T, N>::create(size_t width, size_t height)
    {
        _data.clear();
        _data.resize(N * width * height, typename Texture<T, N>::Value_t(0));
        _size = Vector2ui{width, height};
    }

    template<typename T, size_t N>
    void Texture<T, N>::set_padding_type(PaddingType type)
    {
        _padding_type = type;
    }

    template<typename T, size_t N>
    PaddingType Texture<T, N>::get_padding_type() const
    {
        return _padding_type;
    }

    template<typename T, size_t N>
    Vector2ui Texture<T, N>::get_size() const
    {
        return _size;
    }

    template<typename T, size_t N>
    auto* Texture<T, N>::expose_data()
    {
        return &_data[1];
    }

    template<typename T, size_t N>
    void Texture<T, N>::create_from(const Image<T, N>& image)
    {
        _data.clear();
        _data.reserve(image.get_size().x() * image.get_size().y() * Image<T, N>::Value_t::size());

        for (size_t y = 0; y < image.get_size().y(); y++)
        {
            for (size_t x = 0; x < image.get_size().x(); x++)
            {
                std::vector<size_t> seq;

                // TODO: why is this workaround needed?
                if (N == 1)
                    seq = {0};
                else if (N == 2)
                    seq = {1, 0};
                else if (N == 3)
                    seq = {2, 0, 1};
                else if (N == 4)
                    seq = {2, 0, 1, 3};

                auto px = image.at(x, image.get_size().y() - (y + 1));

                for (size_t i : seq)
                {
                    if (std::is_same_v<T, bool>)
                        _data.push_back(bool(px.at(i)) ? 255 : 0);
                    else
                        _data.push_back(px.at(i));
                }
            }
        }
    }
}