// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/shader.hpp>

namespace crisp
{
    bool Shader::load_from_file(const std::string& path)
    {
        return sf::Shader::loadFromFile(path, sf::Shader::Fragment);
    }

    void Shader::set_int(const std::string& var_name, khronos_int32_t v)
    {
        sf::Shader::setUniform(var_name, v);
    }

    void Shader::set_float(const std::string& var_name, khronos_float_t v)
    {
        sf::Shader::setUniform(var_name, v);
    }

    void Shader::set_bool(const std::string& var_name, bool v)
    {
        sf::Shader::setUniform(var_name, v);
    }

    template<typename T>
    void Shader::set_vec2(const std::string& var_name, const crisp::Vector<T, 2>& vec)
    {
        sf::Shader::setUniform(var_name, sf::Glsl::Vec2(static_cast<float>(vec.x()), static_cast<float>(vec.y())));
    }

    template<typename T>
    void Shader::set_vec3(const std::string& var_name, const crisp::Vector<T, 3>& vec)
    {
        sf::Shader::setUniform(var_name,
            sf::Glsl::Vec3(
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.y())
            )
        );
    }

    template<typename T>
    void Shader::set_vec4(const std::string& var_name, const crisp::Vector<T, 4>& vec)
    {
        sf::Shader::setUniform(var_name,
            sf::Glsl::Vec4(
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.z()),
                static_cast<float>(vec.w())
            )
        );
    }

    void Shader::set_vec3(const std::string& var_name, const RGB& color)
    {
        sf::Shader::setUniform(var_name,
            sf::Glsl::Vec3(
                static_cast<float>(color.red()),
                static_cast<float>(color.green()),
                static_cast<float>(color.blue())
            )
        );
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<T>& matrix)
    {
        std::vector<float> data;
        data.reserve(matrix.rows() * matrix.cols());

        for (size_t y = 0; y < matrix.cols(); ++y)
            for (size_t x = 0; x < matrix.rows(); ++x)
                data.push_back(matrix(x, y));

        sf::Shader::setUniformArray(var_name, &data[0], matrix.rows() * matrix.cols());
        set_int(var_name + "_rows", matrix.rows());
        set_int(var_name + "_cols", matrix.cols());
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 2>>& matrix)
    {
        std::vector<sf::Glsl::Vec2> data;
        data.reserve(matrix.rows() * matrix.cols());

        for (size_t y = 0; y < matrix.cols(); ++y)
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                data.emplace_back(
                    static_cast<float>(matrix(x, y).x()),
                    static_cast<float>(matrix(x, y).y())
                );
            }

        sf::Shader::setUniformArray(var_name, &data[0], matrix.rows() * matrix.cols());
        set_int(var_name + "_rows", matrix.rows());
        set_int(var_name + "_cols", matrix.cols());
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 3>>& matrix)
    {
        std::vector<sf::Glsl::Vec2> data;
        data.reserve(matrix.rows() * matrix.cols());

        for (size_t y = 0; y < matrix.cols(); ++y)
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                data.emplace_back(
                    static_cast<float>(matrix(x, y).x()),
                    static_cast<float>(matrix(x, y).y()),
                    static_cast<float>(matrix(x, y).z())
                );
            }

        sf::Shader::setUniformArray(var_name, &data[0], matrix.rows() * matrix.cols());
        set_int(var_name + "_rows", matrix.rows());
        set_int(var_name + "_cols", matrix.cols());
    }

    template<typename T>
    void Shader::set_matrix(const std::string& var_name, const Matrix<Vector<T, 4>>& matrix)
    {
        std::vector<sf::Glsl::Vec2> data;
        data.reserve(matrix.rows() * matrix.cols());

        for (size_t y = 0; y < matrix.cols(); ++y)
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                data.emplace_back(
                    static_cast<float>(matrix(x, y).x()),
                    static_cast<float>(matrix(x, y).y()),
                    static_cast<float>(matrix(x, y).z()),
                    static_cast<float>(matrix(x, y).w())
                );
            }

        sf::Shader::setUniformArray(var_name, &data[0], matrix.rows() * matrix.cols());
        set_int(var_name + "_rows", matrix.rows());
        set_int(var_name + "_cols", matrix.cols());
    }

    void Shader::set_kernel(const std::string& var_name, const Kernel& kernel)
    {
        set_matrix<float>(var_name, static_cast<Matrix<float>>(kernel));
    }

    void Shader::set_structuring_element(const std::string& var_name, const StructuringElement& se)
    {
        std::vector<float> data;
        data.reserve(se.rows() * se.cols());

        for (size_t y = 0; y < se.cols(); ++y)
            for (size_t x = 0; x < se.rows(); ++x)
            {
                auto val = se(x, y);

                if (val.has_value())
                    data.push_back(val.value());
                else
                    data.push_back(-1.f);
            }

        sf::Shader::setUniformArray(var_name, &data[0], se.rows() * se.cols());
        set_int(var_name + "_rows", se.rows());
        set_int(var_name + "_cols", se.cols());
    }

    void Shader::set_active(bool b)
    {
        if (b)
            sf::Shader::bind(this);
        else
            sf::Shader::bind(nullptr);
    }

    ID Shader::get_native_handle() const
    {
        return _id;
    }
}
