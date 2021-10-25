// 
// Copyright 2021 Clemens Cords
// Created on 25.10.21 by clem (mail@clemens-cords.com)
//

#include <gpu_side/texture.hpp>
#include <gpu_side/state.hpp>
#include <GL/gl.h>

namespace crisp
{
    template<typename T, size_t N>
    Texture<T, N>::~Texture()
    {
        State::free_texture(_handle);
    }

    template<typename T, size_t N>
    Texture<T, N>::Texture(GLNativeHandle already_allocated_handle)
    {
        auto info = State::get_texture_info(already_allocated_handle);
        assert(N == info.n_planes && "Number of planes are different from allocated texture");

        if (std::is_same_v<float, T>)
            assert(info.type == GL_FLOAT && "Data type different from allocated texture");
        else
            assert(info.type == GL_UNSIGNED_BYTE && "Data type different from allocated texture");

        _handle = already_allocated_handle;
        auto size = State::get_texture_size(already_allocated_handle);
        _width = size.x();
        _height = size.y();
    }

    template<typename T, size_t N>
    Texture<T, N>::Texture(const Image<T, N>& img)
    {
        create_from(img);
    }

    template<typename T, size_t N>
    Texture<T, N>::Texture(size_t width, size_t height)
    {
        create(width, height);
    }

    template<typename T, size_t N>
    void Texture<T, N>::create_from(const Image<T, N>& img)
    {
        _width = img.get_size().x();
        _height = img.get_size().y();
        _handle = State::register_texture<T, N>(img);
    }

    template<typename T, size_t N>
    void Texture<T, N>::create(size_t width, size_t height)
    {
        State::register_texture<T, N>(width, height);
    }

    template<typename T, size_t N>
    Image<T, N> Texture<T, N>::to_image() const
    {
        if (_handle == NONE)
            return Image<T, N>(_width, _height, 0);

        auto data = State::get_texture_data(_handle);
        auto out = Image<T, N>(_width, _height, -1);

        size_t todo = data.size();
        assert(data.size() == _width * _height * N);

        for (size_t y = 0, i = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
                Vector<T, N> to_push;
                for (size_t n = 0; n < N; ++n)
                    to_push.at(n) = data[i + n];

                out(x, y) = to_push;
                i += N;
            }
        }

        return out;
    }
}

