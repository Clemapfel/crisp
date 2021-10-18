// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/shader.hpp>
#include <texture/state.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>

namespace crisp
{
    Shader::Shader(const std::string& path)
    {
        _shader_id = State::register_shader(path);
        _program_id = State::register_program(_shader_id);
    }

    Shader::~Shader()
    {
        for (auto& entry : _var_name_to_proxy)
        {
            switch (entry.second.type)
            {
                case INT:
                    State::free_int(entry.second.id);
                    break;
                case FLOAT:
                    State::free_float(entry.second.id);
                    break;
                case BOOL:
                    State::free_bool(entry.second.id);
                    break;
                case VEC2:
                    State::free_vec2(entry.second.id);
                    break;
                case VEC3:
                    State::free_vec3(entry.second.id);
                    break;
                case VEC4:
                    State::free_vec4(entry.second.id);
                    break;
                case MATRIX:
                    State::free_matrix(entry.second.id);
                    break;
                case ARRAY_VEC1:
                    State::free_array<1>(entry.second.id);
                    break;
                case ARRAY_VEC2:
                    State::free_array<2>(entry.second.id);
                    break;
                case ARRAY_VEC3:
                    State::free_array<3>(entry.second.id);
                    break;
                case ARRAY_VEC4:
                    State::free_array<4>(entry.second.id);
                    break;
                case TEXTURE:
                    // sic, noop
                    break;
            }
        }

        State::free_shader(_shader_id);
        State::free_program(_program_id);
    }

    void Shader::bind_uniforms()
    {
        size_t texture_location = 0;
        for (auto& pair : _var_name_to_proxy)
        {
            switch (pair.second.type)
            {
                case INT:
                    State::bind_int(_program_id, pair.first, pair.second.id);
                    break;
                case FLOAT:
                    State::bind_float(_program_id, pair.first, pair.second.id);
                    break;
                case BOOL:
                    State::bind_bool(_program_id, pair.first, pair.second.id);
                    break;
                case VEC2:
                    State::bind_vec2(_program_id, pair.first, pair.second.id);
                    break;
                case VEC3:
                    State::bind_vec3(_program_id, pair.first, pair.second.id);
                    break;
                case VEC4:
                    State::bind_vec4(_program_id, pair.first, pair.second.id);
                    break;
                case MATRIX:
                    State::bind_matrix(_program_id, pair.first, pair.second.id);
                    break;
                case ARRAY_VEC1:
                    State::bind_array<1>(_program_id, pair.first, pair.second.id);
                    break;
                case ARRAY_VEC2:
                    State::bind_array<2>(_program_id, pair.first, pair.second.id);
                    break;
                case ARRAY_VEC3:
                    State::bind_array<3>(_program_id, pair.first, pair.second.id);
                    break;
                case ARRAY_VEC4:
                    State::bind_array<4>(_program_id, pair.first, pair.second.id);
                    break;
                case TEXTURE:
                    State::bind_texture(_program_id, pair.first, pair.second.id, texture_location);
                    texture_location += 1;
                    break;
            }
        }
    }

    void Shader::set_active()
    {
        State::bind_shader_program(_program_id);
        State::set_active_shader(this);
    }

    void Shader::set_int(const std::string& var_name, int32_t v)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry{
                .id = State::register_int(v),
                .type = ProxyType::INT
            }
        });
    }

    void Shader::set_bool(const std::string& var_name, bool v)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry{
                .id = State::register_bool(v),
                .type = ProxyType::BOOL
            }
        });
    }

    void Shader::set_float(const std::string& var_name, float v)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry{
                .id = State::register_float(v),
                .type = ProxyType::FLOAT
            }
        });
    }

    template<typename T>
    void Shader::set_vec2(const std::string& var_name, const crisp::Vector<T, 2>& vec)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec2(vec),
                .type = ProxyType::VEC2
            }
        });
    }

    template<typename T>
    void Shader::set_vec3(const std::string& var_name, const crisp::Vector<T, 3>& vec)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec3(vec),
                .type = ProxyType::VEC3
            }
        });
    }

    template<typename T>
    void Shader::set_vec4(const std::string& var_name, const crisp::Vector<T, 4>& vec)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec4(vec),
                .type = ProxyType::VEC4
            }
        });
    }

    template<typename T, size_t n_rows, size_t n_cols>
    void Shader::set_matrix(const std::string& var_name, const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& matrix)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_matrix<T, n_rows, n_cols>(matrix),
                .type = ProxyType::MATRIX
            }
        });
    }

    template<typename T>
    void Shader::set_array(const std::string& var_name, const std::vector<T>& data)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_array(data),
                .type = ProxyType::ARRAY_VEC1
            }
        });
    }

    template<typename T>
    void Shader::set_array_vec2(const std::string& var_name, const std::vector<crisp::Vector<T, 2>>& data)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec2_array(data),
                .type = ProxyType::ARRAY_VEC2
            }
        });
    }

    template<typename T>
    void Shader::set_array_vec3(const std::string& var_name, const std::vector<crisp::Vector<T, 3>>& data)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec3_array(data),
                .type = ProxyType::ARRAY_VEC3
            }
        });
    }

    template<typename T>
    void Shader::set_array_vec4(const std::string& var_name, const std::vector<crisp::Vector<T, 4>>& data)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec4_array(data),
                .type = ProxyType::ARRAY_VEC4
            }
        });
    }


    void Shader::set_color(const std::string& var_name, const crisp::RGB& rgb)
    {
        auto as_vec = Vector<float, 3>{rgb.red(), rgb.green(), rgb.blue()};
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec3(as_vec),
                .type = ProxyType::VEC3
            }
        });
    }

    /*
    void Shader::set_kernel(const std::string& var_name, const Kernel& kernel)
    {
        auto as_matrix = static_cast<Matrix<float>>(kernel);
        set_matrix(var_name, as_matrix);
    }

    void Shader::set_structuring_element(const std::string& var_name, const StructuringElement& se)
    {
        auto as_matrix = Matrix<float>(se.rows(), se.cols());

        for (size_t x = 0; x < se.rows(); ++x)
        {
            for (size_t y = 0; y < se.cols(); ++y)
            {
                auto val = se(x, y);

                if (val.has_value())
                    as_matrix(x, y) = static_cast<float>(val.value());
                else
                    as_matrix(x, y) = -1;
            }
        }

        set_matrix(var_name, as_matrix);
    }
     */

    template<typename Value_t, size_t N>
    void Shader::set_texture(const std::string& var_name, const Image<Value_t, N>& tex)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = static_cast<ProxyID>(State::register_texture(tex)),
                .type = ProxyType::TEXTURE
            }
        });
    }
}