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
        _data.resize(N * width * height, T(0));
    }

    template<typename T, size_t N>
    void Texture<T, N>::create(size_t width, size_t height)
    {
        _data.clear();
        _data.resize(N * width * height, T(0));
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
    T* Texture<T, N>::expose_data()
    {
        return &_data[0];
    }


    template<typename T, size_t N>
    void Texture<T, N>::create_from(const Image<T, N>& image)
    {
        _data.clear();
        for (size_t y = 0; y < image.get_size().y(); ++y)
        {
            for (size_t x = 0; x < image.get_size().x(); ++x)
            {
                auto px = image(x, image.get_size().y() - y);

                for (size_t i = 0; i < N; ++i)
                    _data.push_back(px.at(i));
            }
        }
    }
}