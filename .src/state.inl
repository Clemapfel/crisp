// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/state.hpp>

namespace crisp
{
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
        char compilation_log[512] = "";
        glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_success);
        glGetShaderInfoLog(id, 512, nullptr, compilation_log);

        if (not bool(compilation_success))
        {
            std::cerr << "[WARNING] Failed to compile shader at " << path << std::endl;
            std::cerr << "Log: " << compilation_log << std::endl;
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
        char compilation_log[512] = "";
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_success);
        glGetShaderInfoLog(shader_id, 512, nullptr, compilation_log);

        if (not bool(compilation_success))
        {
            std::cerr << "[WARNING] Failed to compile shader at " << path << std::endl;
            std::cerr << "Log: " << compilation_log << std::endl;
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
        char linking_log[512] = "";
        glGetProgramiv(program_id, GL_LINK_STATUS, &linking_success);

        if (not (bool(linking_success)))
        {
            std::cerr << "[WARNING] Failed to link shader to program " << program_id << std::endl;
            std::cerr << "Log: " << linking_log << std::endl;
        }

        _shader_programs.insert(program_id);
        return program_id;
    }

    void State::free_program(GLNativeHandle id)
    {
        _shader_programs.erase(id);
        glDeleteProgram(id);
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
        _ints.insert({id, v});
        return id;
    }

    ProxyID State::register_bool(bool v)
    {
        auto id = get_next_id();
        _ints.insert({id, v});
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

    template<typename T>
    ProxyID State::register_matrix(const Matrix<T>& matrix)
    {
        auto id = get_next_id();
        _array_vec1s.insert({
           id,
           std::vector<float>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec1s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back(matrix(x, y));
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 2>>& matrix)
    {
        auto id = get_next_id();
        _array_vec2s.insert({
           id,
           std::vector<std::array<float, 2>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec2s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                });
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 3>>& matrix)
    {
        auto id = get_next_id();
        _array_vec3s.insert({
           id,
           std::vector<std::array<float, 3>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec3s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                    static_cast<float>(matrix(x, y).at(2)),
                });
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 4>>& matrix)
    {
        auto id = get_next_id();
        _array_vec4s.insert({
           id,
           std::vector<std::array<float, 4>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec4s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                    static_cast<float>(matrix(x, y).at(2)),
                    static_cast<float>(matrix(x, y).at(3)),
                });
            }
        }

        return id;
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

    template<size_t N>
    void State::free_matrix(ProxyID id)
    {
        static_assert(0 < N and N <= 4);

        switch(N)
        {
            case 1:
                _array_vec1s.erase(id);
                return;
            case 2:
                _array_vec2s.erase(id);
                return;
            case 3:
                _array_vec3s.erase(id);
                return;
            case 4:
                _array_vec4s.erase(id);
                return;
        };
    }










}