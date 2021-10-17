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

    }

    Shader::~Shader()
    {
        for (std::pair<std::string, ProxyEntry>& entry : _var_name_to_proxy)
        {
            switch (entry.second.type)
            {
                case INT:
                    State::free_int(entry.first);
                    break;
                case FLOAT:
                    State::free_float(entry.first);
                    break;
                case BOOL:
                    State::free_bool(entry.first);
                    break;
                case VEC2:
                    State::free_vec2(entry.first);
                    break;
                case VEC3:
                    State::free_vec3(entry.first);
                    break;
                case VEC4:
                    State::free_vec4(entry.first);
                    break;
                case ARRAY_VEC1:
                    State::free_matrix<1>(entry.first);
                    break;
                case ARRAY_VEC2:
                    State::free_matrix<2>(entry.first);
                    break;
                case ARRAY_VEC3:
                    State::free_matrix<3>(entry.first);
                    break;
                case ARRAY_VEC4:
                    State::free_matrix<4>(entry.first);
                    break;
                case TEXTURE:
                    // sic, noop
                    break;
            }
        }
    }

    bool Shader::load_from_file(const std::string& path)
    {
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

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<T>& matrix)
    {
        _var_name_to_proxy.insert({
           var_name,
           ProxyEntry {
               .id = State::register_matrix(matrix),
               .type = ProxyType::ARRAY_VEC1
           }
        });
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 2>>& matrix)
    {
        _var_name_to_proxy.insert({
           var_name,
           ProxyEntry {
               .id = State::register_matrix(matrix),
               .type = ProxyType::ARRAY_VEC2
           }
        });
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 3>>& matrix)
    {
        _var_name_to_proxy.insert({
           var_name,
           ProxyEntry {
               .id = State::register_matrix(matrix),
               .type = ProxyType::ARRAY_VEC3
           }
        });
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 4>>& matrix)
    {
        _var_name_to_proxy.insert({
           var_name,
           ProxyEntry {
               .id = State::register_matrix(matrix),
               .type = ProxyType::ARRAY_VEC4
           }
        });
    }

    void Shader::set_color(std::string& var_name, const RGB& rgb)
    {
        auto as_vec = Vector<float, 3>{rbg.red(), rgb.green(), rgb.blue()};
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_vec3(as_vec),
                .type = ProxyType::VEC3
            }
        });
    }

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

    template<typename Value_t, size_t N>
    void Shader::set_texture(const std::string& var_name, const Texture<Value_t, N>& tex)
    {
        _var_name_to_proxy.insert({
            var_name,
            ProxyEntry {
                .id = State::register_texture(tex),
                .type = ProxyType::TEXTURE
            }
        });
    }
}