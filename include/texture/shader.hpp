// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/Graphics/Shader.hpp>

#include <GLES3/gl3.h>

#include <texture/texture.hpp>
#include <texture/native_handle.hpp>
#include <spatial_filter.hpp>
#include <morphological_transform.hpp>

namespace crisp
{
    template<typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    // fragment shader
    class Shader : private sf::Shader
    {
        public:
            void set_active(bool);
            ID get_native_handle() const;

            bool load_from_file(const std::string& path);

            void set_int(const std::string& var_name, khronos_int32_t);
            void set_float(const std::string& var_name, khronos_float_t);
            void set_bool(const std::string& var_name, bool);

            template<typename Value_t, size_t N>
            void set_texture(const std::string& var_name, const Texture<Value_t, N>&);

            template<typename T>
            void set_vec2(const std::string& var_name, const crisp::Vector<T, 2>&);

            template<typename T>
            void set_vec3(const std::string& var_name, const crisp::Vector<T, 3>&);
            void set_vec3(const std::string& var_name, const crisp::RGB&);

            template<typename T>
            void set_vec4(const std::string& var_name, const crisp::Vector<T, 4>&);

            template<typename T>
            void set_matrix(const std::string& var_name, const Matrix<T>&);

            template<typename T>
            void set_matrix(const std::string& var_name, const Matrix<Vector<T, 2>>&);

            template<typename T>
            void set_matrix(const std::string& var_name, const Matrix<Vector<T, 3>>&);

            template<typename T>
            void set_matrix(const std::string& var_name, const Matrix<Vector<T, 4>>&);

            void set_kernel(const std::string& var_name, const Kernel&);

            void set_structuring_element(const std::string& var_name, const StructuringElement&);

        private:
            ID _id;
    };
}

#include ".src/shader.inl"