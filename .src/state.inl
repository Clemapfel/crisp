// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#include <gpu_side/state.hpp>
#include <gpu_side/shader.hpp>
#include <boost/container/vector.hpp>

namespace crisp
{
    void State::bind_shader(crisp::Shader*)
    {
        _active_shader = shader;
    }

    void State::display()
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(_vertex_array);

        if (_active_shader)
            _active_shader->bind_uniforms();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    template<typename T>
    void State::get_pixel_buffer(Image<T, 1>& image, Vector2ui top_left)
    {
        std::vector<float> buffer;
        buffer.resize(image.get_size().x() * image.get_size().y() * 4);

        glReadPixels(top_left.x(), top_left.y(), image.get_size().x(), image.get_size().y(), GL_RGBA, GL_FLOAT, &buffer[0]);

        for (size_t y = 0, i = 0; y < image.get_size().y(); ++y)
        {
            for (size_t x = 0; x < image.get_size().x(); ++x, i += 4)
            {
                float r = buffer.at(i+0);
                float g = buffer.at(i+1);
                float b = buffer.at(i+2);
                float a = buffer.at(i+3);

                image(x, image.get_size().y() - (y+1)) = r;
            }
        }
    }

    template<typename T>
    void State::get_pixel_buffer(Image<T, 2>& image, Vector2ui top_left)
    {
        std::vector<float> buffer;
        buffer.resize(image.get_size().x() * image.get_size().y() * 4);

        glReadPixels(top_left.x(), top_left.y(), image.get_size().x(), image.get_size().y(), GL_RGBA, GL_FLOAT, &buffer[0]);

        for (size_t y = 0, i = 0; y < image.get_size().y(); ++y)
        {
            for (size_t x = 0; x < image.get_size().x(); ++x, i += 4)
            {
                float r = buffer.at(i+0);
                float g = buffer.at(i+1);
                float b = buffer.at(i+2);
                float a = buffer.at(i+3);

                image(x, image.get_size().y() - (y+1)) = Vector<T, 2>{static_cast<T>(r), static_cast<T>(g)};
            }
        }
    }

    template<typename T>
    void State::get_pixel_buffer(Image<T, 3>& image, Vector2ui top_left)
    {
        std::vector<float> buffer;
        buffer.resize(image.get_size().x() * image.get_size().y() * 4);

        glReadPixels(top_left.x(), top_left.y(), image.get_size().x(), image.get_size().y(), GL_RGBA, GL_FLOAT, &buffer[0]);

        for (size_t y = 0, i = 0; y < image.get_size().y(); ++y)
        {
            for (size_t x = 0; x < image.get_size().x(); ++x, i += 4)
            {
                float r = buffer.at(i+0);
                float g = buffer.at(i+1);
                float b = buffer.at(i+2);
                float a = buffer.at(i+3);

                image(x, image.get_size().y() - (y+1)) = Vector<T, 3>{static_cast<T>(r), static_cast<T>(g), static_cast<T>(g)};
            }
        }
    }

    template<typename T>
    void State::get_pixel_buffer(Image<T, 4>& image, Vector2ui top_left)
    {
        std::vector<float> buffer;
        buffer.resize(image.get_size().x() * image.get_size().y() * 4);

        glReadPixels(top_left.x(), top_left.y(), image.get_size().x(), image.get_size().y(), GL_RGBA, GL_FLOAT, &buffer[0]);

        for (size_t y = 0, i = 0; y < image.get_size().y(); ++y)
        {
            for (size_t x = 0; x < image.get_size().x(); ++x, i += 4)
            {
                float r = buffer.at(i+0);
                float g = buffer.at(i+1);
                float b = buffer.at(i+2);
                float a = buffer.at(i+3);

                image(x, image.get_size().y() - (y+1)) = Vector<T, 4>{static_cast<T>(r), static_cast<T>(g), static_cast<T>(b), static_cast<T>(a)};
            }
        }
    }

    void State::initialize_vertex_shader()
    {
        if (_vertex_shader != -1)
            return;

        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::string path = "/home/clem/Workspace/crisp/include/texture/.shaders/noop.vert";
        std::string source;
        try
        {
            file.open(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            source = buffer.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cerr << "[ERROR] Failure to read shader at " << path << std::endl;
            std::cerr << e.what() << std::endl;
            throw e;
        }

        const char* source_ptr = source.c_str();
        auto id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(id, 1, &source_ptr, nullptr);
        glCompileShader(id);

        int compilation_success;
        char compilation_log[1024] = "";
        glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_success);
        glGetShaderInfoLog(id, 1024, nullptr, compilation_log);

        if (not bool(compilation_success))
        {
            std::cerr << "[WARNING] Failed to compile shader at " << path << std::endl;
            std::cerr << compilation_log << std::endl;
        }

        _vertex_shader = id;
    }

    GLNativeHandle State::register_shader(const std::string& path)
    {
        if (_vertex_shader == -1)
            initialize_vertex_shader();

        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::string source;
        try
        {
            file.open(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            source = buffer.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cerr << "[WARNING] Failure to read shader at " << path << std::endl;
            std::cerr << e.what() << std::endl;
        }

        const char* source_ptr = source.c_str();
        auto shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader_id, 1, &source_ptr, nullptr);
        glCompileShader(shader_id);

        int compilation_success;
        char compilation_log[1024] = "";
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_success);
        glGetShaderInfoLog(shader_id, 1024, nullptr, compilation_log);

        if (compilation_success == GL_FALSE)
        {
            std::cerr << "[WARNING] Failed to compile shader at " << path << std::endl;
            std::cerr << compilation_log << std::endl;
        }

        _fragment_shaders.insert(shader_id);
        return shader_id;
    }

    void State::free_shader(GLNativeHandle id)
    {
        _fragment_shaders.erase(id);
        glDeleteShader(id);
    }

    GLNativeHandle State::register_program(GLNativeHandle fragment_shader_id)
    {
        auto program_id = glCreateProgram();
        glAttachShader(program_id, _vertex_shader);
        glAttachShader(program_id, fragment_shader_id);
        glLinkProgram(program_id);

        int linking_success;
        char linking_log[1024] = "";
        glGetProgramiv(program_id, GL_LINK_STATUS, &linking_success);

        if (linking_success == GL_FALSE)
        {
            std::cerr << "[WARNING] Failed to link shader to program " << program_id << std::endl;
            std::cerr << linking_log << std::endl;
        }

        _shader_programs.insert(program_id);
        return program_id;
    }

    void State::free_program(GLNativeHandle id)
    {
        _shader_programs.erase(id);
        glDeleteProgram(id);
    }

    void State::bind_shader_program(GLNativeHandle program_id)
    {
        glUseProgram(program_id);
        _active_program = program_id;
    }

    template<typename T, size_t N>
    GLNativeHandle State::register_texture(const Image<T, N>& image)
    {
        static_assert(0 < N and N <= 4);


        using Data_t = typename std::conditional<std::is_same_v<T, bool>, GLboolean, float>::type;
        boost::container::vector<Data_t> data;
        data.reserve(image.get_size().x() * image.get_size().y() * N);

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
                        data.push_back(bool(px.at(i)) ? 255 : 0);
                    else
                        data.push_back(px.at(i));
                }
            }
        }

        GLNativeHandle texture_id;
        glGenTextures(1, &texture_id);

        glBindTexture(GL_TEXTURE_2D, texture_id);

        if (not _vertices_initialized)
        {
            // vertex info
            static float vertices[] = {
                // pos, col, tex_coord
                 1,  1, 0,   1, 1, 1,   1, 1, // top right
                 1, -1, 0,   1, 1, 1,   1, 0, // bottom right
                -1, -1, 0,   1, 1, 1,   0, 0, // bottom left
                -1,  1, 0,   1, 1, 1,   0, 1  // top left
            };

            static unsigned int indices[] = {
                    0, 1, 3,
                    1, 2, 3
            };

            glGenVertexArrays(1, &_vertex_array);
            glGenBuffers(1, &_vertex_buffer);
            glGenBuffers(1, &_element_buffer);

            glBindVertexArray(_vertex_array);

            glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            _vertices_initialized = true;
        }

        GLint internal_format;
        GLint format;

        size_t alignment_n = 1;

        if (N == 1)
        {
            format = GL_RED;
            alignment_n = 1;
            if (std::is_same_v<T, bool>)
                internal_format = GL_R8;
            else
                internal_format = GL_R32F;
        }
        else if (N == 2)
        {
            format = GL_RG;
            alignment_n = 2;
            if (std::is_same_v<T, bool>)
                internal_format = GL_RG8;
            else
                internal_format = GL_RG32F;
        }
        else if (N == 3)
        {
            format = GL_RGB;
            alignment_n = 1;
            if (std::is_same_v<T, bool>)
                internal_format = GL_RGB8;
            else
                internal_format = GL_RGB32F;
        }
        else if (N == 4)
        {
            format = GL_RGBA;
            alignment_n = 4;
            if (std::is_same_v<T, bool>)
                internal_format = GL_RGBA8;
            else
                internal_format = GL_RGBA32F;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment_n);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     internal_format,
                     static_cast<GLsizei>(image.get_size().x()),
                     static_cast<GLsizei>(image.get_size().y()),
                     0,
                     format,
                     std::is_same_v<T, bool> ? GL_UNSIGNED_BYTE : GL_FLOAT,
                     &data[0]);

        _textures.insert(texture_id);
        _texture_info.insert({
            texture_id,
            TextureInfo{
                .padding_type = image.get_padding_type(),
                .width = image.get_size().x(),
                .height = image.get_size().y(),
            }
        });

        return texture_id;
    }

    void State::free_texture(GLNativeHandle id)
    {
        glDeleteTextures(1, &id);
    }

    ProxyID State::register_int(int v)
    {
        auto id = get_next_id();
        _ints.insert({id, v});
        return id;
    }

    ProxyID State::register_float(float v)
    {
        auto id = get_next_id();
        _floats.insert({id, v});
        return id;
    }

    ProxyID State::register_bool(bool v)
    {
        auto id = get_next_id();
        _bools.insert({id, v});
        return id;
    }

    template<typename T>
    ProxyID State::register_vec2(const Vector<T, 2>& vec)
    {
        auto id = get_next_id();
        _vec2s.insert({
            id,
            std::array<float, 2>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y())
            }
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec3(const Vector<T, 3>& vec)
    {
        auto id = get_next_id();
        _vec3s.insert({
            id,
            std::array<float, 3>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.z())
            }
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec4(const Vector<T, 4>& vec)
    {
        auto id = get_next_id();
        _vec4s.insert({
            id,
            std::array<float, 4>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.z()),
                static_cast<float>(vec.w())
            }
        });

        return id;
    }

    template<typename T, size_t n_rows, size_t n_cols>
    ProxyID State::register_matrix(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& matrix)
    {
        static_assert(2 <= n_rows and n_rows <= 4 and 2 <= n_cols and n_cols <= 4);
        auto id = get_next_id();

        std::vector<float> as_float;
        as_float.reserve(matrix.rows() * matrix.cols());

            for (size_t y = 0; y < matrix.cols(); ++y)
                for (size_t x = 0; x < matrix.rows(); ++x)
                    as_float.push_back(static_cast<float>(matrix(x, y)));

        _mats.insert({
            id,
            MatrixProxy {
                .data = as_float,
                .n_rows = matrix.rows(),
                .n_cols = matrix.cols()
            }
        });

        return id;
    }

    void State::bind_matrix(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        auto matrix = _mats.at(proxy_id);
        auto location = glGetUniformLocation(program_id, var_name.c_str());

        if (matrix.n_rows == 2 and matrix.n_cols == 2)
            glUniformMatrix2fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 2 and matrix.n_cols == 3)
            glUniformMatrix2x3fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 2 and matrix.n_cols == 4)
            glUniformMatrix2x4fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 3 and matrix.n_cols == 2)
            glUniformMatrix3x2fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 3 and matrix.n_cols == 3)
            glUniformMatrix3fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 3 and matrix.n_cols == 4)
            glUniformMatrix3x4fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 4 and matrix.n_cols == 2)
            glUniformMatrix4x2fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 4 and matrix.n_cols == 3)
            glUniformMatrix4x3fv(location, 1, false, &matrix.data[0]);

        else if (matrix.n_rows == 4 and matrix.n_cols == 4)
            glUniformMatrix4fv(location, 1, false, &matrix.data[0]);
    }

    void State::free_matrix(ProxyID id)
    {
        _mats.erase(id);
    }

    template<typename T>
    ProxyID State::register_array(const std::vector<T>& data)
    {
        std::vector<float> as_float;

        for (const auto& x : data)
            as_float.push_back(static_cast<float>(x));

        auto id = get_next_id();
        _array_vec1s.insert({
            id,
            as_float
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec2_array(const std::vector<crisp::Vector<T, 2>>& data)
    {
        std::vector<float> as_float;

        for (const auto& vec : data)
        {
            as_float.push_back(static_cast<float>(vec.x()));
            as_float.push_back(static_cast<float>(vec.y()));
        }

        auto id = get_next_id();
        _array_vec2s.insert({
            id,
            as_float
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec3_array(const std::vector<crisp::Vector<T, 3>>& data)
    {
        std::vector<float> as_float;

        for (const auto& vec : data)
        {
            as_float.push_back(static_cast<float>(vec.x()));
            as_float.push_back(static_cast<float>(vec.y()));
            as_float.push_back(static_cast<float>(vec.z()));
        }

        auto id = get_next_id();
        _array_vec3s.insert({
            id,
            as_float
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec4_array(const std::vector<crisp::Vector<T, 4>>& data)
    {
        std::vector<float> as_float;

        for (const auto& vec : data)
        {
            as_float.push_back(static_cast<float>(vec.x()));
            as_float.push_back(static_cast<float>(vec.y()));
            as_float.push_back(static_cast<float>(vec.z()));
            as_float.push_back(static_cast<float>(vec.w()));
        }

        auto id = get_next_id();
        _array_vec4s.insert({
            id,
            as_float
        });

        return id;
    }

    template<size_t N>
    void State::free_array(ProxyID id)
    {
        static_assert(1 <= N and N <= 4);

        if (N == 1)
            _array_vec1s.erase(id);
        else if (N == 2)
            _array_vec2s.erase(id);
        else if (N == 3)
            _array_vec3s.erase(id);
        else if (N == 4)
            _array_vec4s.erase(id);
    }

    template<size_t N>
    void State::bind_array(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        auto location = glGetUniformLocation(program_id, var_name.c_str());
        if (N == 1)
            glUniform1fv(location, _array_vec1s.at(proxy_id).size(), &_array_vec1s.at(proxy_id)[0]);
        else if (N == 2)
            glUniform2fv(location, _array_vec2s.at(proxy_id).size(), &_array_vec2s.at(proxy_id)[0]);
        else if (N == 3)
            glUniform3fv(location, _array_vec3s.at(proxy_id).size(), &_array_vec3s.at(proxy_id)[0]);
        else if (N == 4)
            glUniform4fv(location, _array_vec4s.at(proxy_id).size(), &_array_vec4s.at(proxy_id)[0]);
    }

    void State::free_float(ProxyID id)
    {
        _floats.erase(id);
    }

    void State::free_bool(ProxyID id)
    {
        _bools.erase(id);
    }

    void State::free_int(ProxyID id)
    {
        _ints.erase(id);
    }

    void State::free_vec2(ProxyID id)
    {
        _vec2s.erase(id);
    }

    void State::free_vec3(ProxyID id)
    {
        _vec3s.erase(id);
    }

    void State::free_vec4(ProxyID id)
    {
        _vec4s.erase(id);
    }

    void State::bind_int(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        glUniform1i(glGetUniformLocation(program_id, var_name.c_str()), _ints.at(proxy_id));
    }

    void State::bind_float(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        glUniform1f(glGetUniformLocation(program_id, var_name.c_str()), _floats.at(proxy_id));
    }

    void State::bind_bool(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        glUniform1i(glGetUniformLocation(program_id, var_name.c_str()), int(_bools.at(proxy_id)));
    }

    void State::bind_vec2(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        auto& vec = _vec2s.at(proxy_id);
        glUniform2f(glGetUniformLocation(program_id, var_name.c_str()), vec.at(0), vec.at(1));
    }

    void State::bind_vec3(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        auto& vec = _vec3s.at(proxy_id);
        auto location = glGetUniformLocation(program_id, var_name.c_str());
        glUniform3f(glGetUniformLocation(program_id, var_name.c_str()), vec.at(0), vec.at(1), vec.at(2));
    }

    void State::bind_vec4(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id)
    {
        auto& vec = _vec3s.at(proxy_id);
        glUniform4f(glGetUniformLocation(program_id, var_name.c_str()), vec.at(0), vec.at(1), vec.at(2), vec.at(3));
    }

    void State::bind_texture(GLNativeHandle program_id, const std::string& var_name, GLNativeHandle texture_id, size_t texture_unit)
    {
        auto location = glGetUniformLocation(program_id, var_name.c_str());
        glUniform1i(location, texture_unit);
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        auto info = _texture_info.at(texture_id);

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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}