// 
// Copyright 2021 Clemens Cords
// Created on 22.10.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<typename T, size_t N>
    void Workspace::initialize(GLNativeHandle texture)
    {
        _original = texture;
        _texture_a = texture;
        _size = State::get_texture_size(texture);
        _texture_b = State::register_texture<T, N>(_size.x(), _size.y());
        glGenFramebuffers(1, &_framebuffer);
    }

    void Workspace::draw_to_buffer()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
        glBindTexture(GL_TEXTURE_2D, _texture_b);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_b, 0);

        glBindTexture(GL_TEXTURE_2D, _texture_a);
        State::bind_texture(State::get_active_program_handle(), "_texture", _texture_a);
        glViewport(0, 0, _size.x(), _size.y());
        State::display();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        _last_updated = _texture_b;
    }

    void Workspace::swap_buffers()
    {
        auto temp = _texture_b;
        _texture_b = _texture_a;
        _texture_a = temp;

        _n_displays++;
    }

    void Workspace::display()
    {
        draw_to_buffer();
        swap_buffers();
    }

    Workspace::~Workspace()
    {
        glDeleteFramebuffers(1, &_framebuffer);
        glDeleteTextures(1, &_texture_b);
    }

    GLNativeHandle Workspace::yield()
    {
        // paste buffer onto original if it is currently not the buffer
        auto before = State::get_active_program_handle();
        State::bind_shader_program(NONE);
        display();
        State::bind_shader_program(before);

        return _original;
    }
}