// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/state.hpp>

namespace crisp
{
    template<typename CT>
    void State<CT>::enable_context()
    {
        _context.setActive(true);
    }

    template<typename CT>
    void State<CT>::disable_context()
    {
        _context.setActive(false);
    }

    template<typename CT>
    template<typename Value_t, size_t N>
    ID State<CT>::add_texture(Texture<Value_t, N>& texture)
    {
        enable_context();

        ID id;
        glGenTextures(1, &id);
        _info.emplace(id, TextureInfo{std::is_same_v<Value_t, bool>, N, texture.get_padding_type()});

        glBindTexture(GL_TEXTURE_2D, id);

        GLint internal_format;
        GLint format;
        if (N == 1)
        {
            format = GL_RED;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_R32F;
            else if (std::is_same_v<Value_t, bool>)
                internal_format = GL_R8;
        }
        else if (N == 2)
        {
            format = GL_RG;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_RG32F;
            else if (std::is_same_v<Value_t, bool>)
                internal_format = GL_RG8;
        }
        else if (N == 3)
        {
            format = GL_RGB;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_RGB32F;
            else if (std::is_same_v<Value_t, bool>)
                internal_format = GL_R3_G3_B2;
        }
        else if (N == 4)
        {
            format = GL_RGBA;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_RGBA32F;
            else if (std::is_same_v<Value_t, bool>)
                internal_format = GL_RGBA2;
        }


        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     internal_format,
                     texture.get_size().x(),
                     texture.get_size().y(),
                     0,
                     format,
                     (std::is_same_v<Value_t, bool> ? GL_BOOL : GL_FLOAT),
                     texture.expose_data());

        glGenerateMipmap(GL_TEXTURE_2D);

        return id;
    }

    template<typename CT>
    void State<CT>::bind_texture(ID id)
    {
        enable_context();

        auto info = _info.at(id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, padding_type_to_gl_padding(info.padding_type));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, padding_type_to_gl_padding(info.padding_type));

        if (info.padding_type == PaddingType::ZERO)
        {
            float border[] = {0.f, 0.f, 0.f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        }
        else if (info.padding_type == PaddingType::ONE)
        {
            float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);





    }
}