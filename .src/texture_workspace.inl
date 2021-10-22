// 
// Copyright 2021 Clemens Cords
// Created on 22.10.21 by clem (mail@clemens-cords.com)
//

#include <gpu_side/texture_workspace.hpp>

namespace crisp
{
    template<typename T, size_t N>
    void Workspace::initialize(const Image<T, N>& image)
    {
        _texture_a = State::register_texture(image);
        _texture_b = State::register_texture<T, N>(image.get_size().x(), image.get_size().y());
        glGenFramebuffers(1, &_framebuffer);
        _size = image.get_size();
    }

    void Workspace::display()
    {
        glBindTexture(GL_TEXTURE_2D, _texture_b);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_b, 0);
        //assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindTexture(GL_TEXTURE_2D, _texture_a);
        State::bind_texture(State::get_active_program_handle(), "_texture", _texture_a);
        glViewport(0, 0, _size.x(), _size.y());
        State::display();

        auto temp = _texture_b;
        _texture_b = _texture_a;
        _texture_a = temp;

        _n_displays++;
    }

    Workspace::~Workspace()
    {
        glDeleteFramebuffers(1, _framebuffer);
        if (_textures_to_keep.find(_texture_a) != _textures_to_keep.end())
            glDeleteTextures(1, _texture_a);

        if (_textures_to_keep.find(_texture_b) != _textures_to_keep.end())
            glDeleteTextures(1, _texture_b);
    }

    GLNativeHandle Workspace::yield()
    {
        if (_n_displays % 2 == 0)
        {
            glDeleteTextures(1, _texture_a);
            _textures_to_keep.insert(_texture_b);
            return _texture_b;
        }
        else
        {
            glDeleteTextures(1, _texture_b);
            _textures_to_keep.insert(_texture_a);
            return _texture_a;
        }
    }
}