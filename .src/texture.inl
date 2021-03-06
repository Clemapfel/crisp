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
        _workspace.initialize<T, N>(_handle);
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
    Texture<T, N>::Texture(const Texture<T, N>& other)
    {
        _handle = State::register_texture<T, N>(other.get_handle());
        _width = other.get_size().x();
        _height = other.get_size().y();
        _workspace.initialize<T, N>(_handle);
    }

    template<typename T, size_t N>
    Texture<T, N>& Texture<T, N>::operator=(const Texture<T, N>& other)
    {
        _handle = State::register_texture<T, N>(other.get_handle());
        _width = other.get_size().x();
        _height = other.get_size().y();
        _workspace.initialize<T, N>(_handle);

        return *this;
    }

    template<typename T, size_t N>
    void Texture<T, N>::create_from(const Image<T, N>& img)
    {
        _width = img.get_size().x();
        _height = img.get_size().y();
        _handle = State::register_texture<T, N>(img);
        _workspace.initialize<T, N>(_handle);
    }

    template<typename T, size_t N>
    void Texture<T, N>::create(size_t width, size_t height)
    {
        State::register_texture<T, N>(width, height);
        _workspace.initialize<T, N>(_handle);
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

                out(x, _height - y) = to_push;
                i += N;
            }
        }

        return out;
    }

    template<typename T, size_t N>
    Vector2ui Texture<T, N>::get_size() const
    {
        return Vector2ui{_width, _height};
    }

    template<typename T, size_t N>
    GLNativeHandle Texture<T, N>::get_handle() const
    {
        return _handle;
    }

    template<typename T, size_t N>
    Texture<T, N>::operator GLNativeHandle()
    {
        return _handle;
    }

    template<typename T, size_t N>
    Workspace& Texture<T, N>::get_workspace()
    {
        return _workspace;
    }

    template<typename T, size_t N>
    GLNativeHandle Texture<T, N>::swap_native_objects(GLNativeHandle new_object)
    {
        auto out = _handle;
        auto info = State::get_texture_info(new_object);
        assert(N == info.n_planes && "Number of planes are different from allocated texture");

        if (std::is_same_v<float, T>)
            assert(info.type == GL_FLOAT && "Data type different from allocated texture");
        else
            assert(info.type == GL_UNSIGNED_BYTE && "Data type different from allocated texture");

        _handle = new_object;
        _width = info.width;
        _height = info.height;
        _workspace.initialize<T, N>(_handle);

        return out;
    }
}

