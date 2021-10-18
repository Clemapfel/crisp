// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

#include <vector.hpp>

namespace crisp
{
    /// @brief fragment shader
    class Shader
    {
        public:
            /// @brief ctor
            /// @param path: absolute path to source code file
            Shader(const std::string& path);
            
            /// @brief dtor, frees related gpu-side uniforms and shader program on destruction
            ~Shader();

            /// @brief bind registered variables to gpu-side uniforms
            void bind_uniforms();

            /// @bief bind shaders as current fragment shader
            void set_active();
            
            /// @brief transform image to texture, allocated gpu-side and bind it to a shader uniform
            /// @param var_name: exact variable name from shader source code
            /// @param image
            template<typename Value_t, size_t N>
            void set_texture(const std::string& var_name, const Image<Value_t, N>&);
            
            /// @brief register int and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param int
            void set_int(const std::string& var_name, int32_t);
            
            /// @brief register float and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param float
            void set_float(const std::string& var_name, float);
            
            /// @brief register bool and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param bool
            void set_bool(const std::string& var_name, bool);

            /// @brief register 2-component vector, transform it to vector of floats and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector
            template<typename T>
            void set_vec2(const std::string& var_name, const crisp::Vector<T, 2>&);

            /// @brief register 3-component vector, transform it to vector of floats and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector
            template<typename T>
            void set_vec3(const std::string& var_name, const crisp::Vector<T, 3>&);
            
            /// @brief register 4-component vector, transform it to vector of floats and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector
            template<typename T>
            void set_vec4(const std::string& var_name, const crisp::Vector<T, 4>&);

            /// @brief register rgb color as 3-component vector and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector
            void set_color(const std::string& var_name, const crisp::RGB&);
            
            /// @brief register matrix and bind it to uniform of type matnxm where n in {2, 3, 4} and m in {2, 3, 4}
            /// @param var_name: exact variable name from shader source code
            /// @param matrix
            /// @tparam T value type of matrix, has to be able to be statically cast to float
            /// @tparam n_cols: number of columns, in {2, 3, 4}
            /// @tparam n_rows: number of rows, in {2, 3, 4}
            template<typename T, size_t n_rows, size_t n_cols>
            void set_matrix(const std::string& var_name, const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&);

            /// @brief register fixed-size array of float and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector, same size as gpu-side array
            template<typename T>
            void set_array(const std::string& var_name, const std::vector<T>&);

            /// @brief register fixed-size array of vec2s and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector, same size as gpu-side array
            template<typename T>
            void set_array_vec2(const std::string& var_name, const std::vector<crisp::Vector<T, 2>>&);

            /// @brief register fixed-size array of vec3s and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector, same size as gpu-side array
            template<typename T>
            void set_array_vec3(const std::string& var_name, const std::vector<crisp::Vector<T, 3>>&);

            /// @brief register fixed-size array of vec4s and bind to uniform
            /// @param var_name: exact variable name from shader source code
            /// @param vector, same size as gpu-side array
            template<typename T>
            void set_array_vec4(const std::string& var_name, const std::vector<crisp::Vector<T, 4>>&);
            
        private:
            GLNativeHandle _shader_id,
                           _program_id;

            enum ProxyType
            {
                INT,
                FLOAT,
                BOOL,
                VEC2,
                VEC3,
                VEC4,
                MATRIX,
                ARRAY_VEC1,
                ARRAY_VEC2,
                ARRAY_VEC3,
                ARRAY_VEC4,
                TEXTURE
            };

            struct ProxyEntry
            {
                ProxyID id;
                ProxyType type;
            };

            std::map<std::string, ProxyEntry> _var_name_to_proxy;
    };
}

#include ".src/shader.inl"