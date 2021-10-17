// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>

namespace crisp
{
    class Shader
    {
        public:
            Shader(const std::string& path);
            ~Shader();

            void set_int(const std::string& var_name, int32_t);
            void set_float(const std::string& var_name, float);
            void set_bool(const std::string& var_name, bool);

            template<typename Value_t, size_t N>
            void set_texture(const std::string& var_name, const Texture<Value_t, N>&);

            template<typename T>
            void set_vec2(const std::string& var_name, const crisp::Vector<T, 2>&);

            template<typename T>
            void set_vec3(const std::string& var_name, const crisp::Vector<T, 3>&);
            
            void set_color(const std::string& var_name, const crisp::RGB&);

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
            GLNativeHandle _shader_id,
                           _program_id;

            enum ProxyType
            {
                INT, FLOAT, BOOL, VEC2, VEC3, VEC4, ARRAY_VEC1, ARRAY_VEC2, ARRAY_VEC3, ARRAY_VEC4, TEXTURE
            };

            struct ProxyEntry
            {
                ProxyID id;
                ProxyType type;
            };

            std::unordered_map<std::string, ProxyEntry> _var_name_to_proxy;
    };
}