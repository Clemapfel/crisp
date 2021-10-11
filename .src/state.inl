// 
// Copyright 2021 Clemens Cords
// Created on 09.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/state.hpp>

namespace crisp
{

    template<typename CT>
    template<typename Value_t, size_t N>
    ID State<CT>::add_texture(Texture<Value_t, N>& texture)
    {
        // allocate new context
        size_t current = 0;
        bool found = false;
        for (; current < _proxies_allocated.size(); ++current)
        {
            if (not _proxies_allocated.at(current))
            {
                found = true;
                break;
            }
        }

        if (not found)
        {
            std::cerr << "[WARNING] The maximum number of sf::Textures (" << MAXIMUM_NUMBER_OF_TEXTURES << ") has been reached, allocating new textures may override others and cause problems." << std::endl;
            std::cerr << "Please deallocate some of them before continuing. If absolutely necessary, you can also change the maximum allowable number by modifying MAXIMUM_NUMBER_OF_TEXTURES in /crisp/textures/state.hpp" << std::endl;
            current = 0;
        }


        TextureProxy& proxy = _proxies.at(current);
        proxy.bool_or_float = std::is_same_v<Value_t, bool>;
        proxy.padding_type = texture.get_padding_type();
        proxy.context.create(texture.get_size().x(), texture.get_size().y(), settings);

        ID id;
        glGenTextures(1, &id);

        _native_to_index[id] = current;

        proxy.native_handle = id;

        // texture
        glBindTexture(GL_TEXTURE_2D, id);

        glGenVertexArrays(1, &proxy.vertex_array_id);
        glGenBuffers(1, &proxy.vertex_buffer_id);
        glGenBuffers(1, &proxy.element_buffer_id);

        glBindVertexArray(proxy.vertex_array_id);

        glBindBuffer(GL_ARRAY_BUFFER, proxy.vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, proxy.element_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), element_indices, GL_STATIC_DRAW);

        // position -> 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);

        // color -> 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord -> 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // texture
        glBindTexture(GL_TEXTURE_2D, id);

        GLint internal_format;
        GLint format;
        if (N == 1)
        {
            format = GL_RED;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_R32F;
            else if (std::is_same_v<Value_t, bool>)
            {
                internal_format = GL_R8;
            }
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
                internal_format = GL_RGB8;
        }
        else if (N == 4)
        {
            format = GL_RGBA;
            if (std::is_same_v<Value_t, float>)
                internal_format = GL_RGBA32F;
            else if (std::is_same_v<Value_t, bool>)
                internal_format = GL_RGBA8;
        }

        glTexImage2D(GL_TEXTURE_2D,
             0,
             internal_format,
             texture.get_size().x(),
             texture.get_size().y(),
             0,
             format,
             (std::is_same_v<Value_t, bool> ? GL_BYTE : GL_FLOAT),
             texture.expose_data());

        glGenerateMipmap(GL_TEXTURE_2D);
        proxy.context.setActive(false);

        return id;
    }

    template<typename CT>
    void State<CT>::bind_texture(ID id)
    {
        auto& proxy = _proxies.at(id);
        proxy.context.setActive(true);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, padding_type_to_gl_padding(proxy.padding_type));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, padding_type_to_gl_padding(proxy.padding_type));

        if (proxy.padding_type == PaddingType::ZERO)
        {
            float border[] = {0.f, 0.f, 0.f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        }
        else if (proxy.padding_type == PaddingType::ONE)
        {
            float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}