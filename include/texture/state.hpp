// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <set>

#include <SFML/Graphics/Glsl.hpp>

#include <vector.hpp>
#include <padding_type.hpp>
#include <texture/native_handle.hpp>
#include <texture/texture.hpp>

namespace crisp
{
    using ProxyID = int;    // -1, -2, ... so it isn't confused with GLNativeHandle

    template<typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    class Shader;

    // holds proxies to all objects relevant for currently registered shaders
    struct State
    {
        friend class Shader;

        State() = delete;

        template<typename T>
        static void get_pixel_buffer(Image<T, 1>& image, Vector2ui top_left = {0, 0});

        template<typename T>
        static void get_pixel_buffer(Image<T, 2>& image, Vector2ui top_left = {0, 0});

        template<typename T>
        static void get_pixel_buffer(Image<T, 3>& image, Vector2ui top_left = {0, 0});

        template<typename T>
        static void get_pixel_buffer(Image<T, 4>& image, Vector2ui top_left = {0, 0});

        static void display();
        static void set_active_shader(crisp::Shader*);

        static ProxyID register_int(int);
        static void free_int(ProxyID);
        static void bind_int(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

        static ProxyID register_float(float);
        static void free_float(ProxyID);
        static void bind_float(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

        static ProxyID register_bool(bool);
        static void free_bool(ProxyID);
        static void bind_bool(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

        template<typename T>
        static ProxyID register_vec2(const Vector<T, 2>&);

        static void free_vec2(ProxyID);
        static void bind_vec2(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);
        
        template<typename T>
        static ProxyID register_vec3(const Vector<T, 3>&);

        static void free_vec3(ProxyID);
        static void bind_vec3(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

        template<typename T>
        static ProxyID register_vec4(const Vector<T, 4>&);

        static void free_vec4(ProxyID);
        static void bind_vec4(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);
        
        template<typename T>
        static ProxyID register_matrix(const Matrix<T>&);

        template<typename T>
        static ProxyID register_matrix(const Matrix<Vector<T, 2>>&);

        template<typename T>
        static ProxyID register_matrix(const Matrix<Vector<T, 3>>&);

        template<typename T>
        static ProxyID register_matrix(const Matrix<Vector<T, 4>>&);

        /// @tparam N: number of components in inner vector
        template<size_t N>
        static void free_matrix(ProxyID);
        
        template<size_t N>
        static void bind_matrix(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

        /// @param path: [in]
        /// @param program_id: [out]
        /// @param shader_id: [out]
        static GLNativeHandle register_shader(const std::string& path);
        static void free_shader(GLNativeHandle);

        static GLNativeHandle register_program(GLNativeHandle fragment_shader_id);
        static void free_program(GLNativeHandle);
        static void bind_shader_program(GLNativeHandle program_id);

        template<typename T, size_t N>
        static GLNativeHandle register_texture(Texture<T, N>&);
        void free_texture(GLNativeHandle);
        static void bind_texture(GLNativeHandle program_id, const std::string& var_name, GLNativeHandle texture_id, size_t texture_location);

        private:

            static inline ProxyID _current = 0;
            static ProxyID get_next_id()
            {
                return _current--;
            }

            // primitives
            static inline std::unordered_map<ProxyID, int> _ints = {};
            static inline std::unordered_map<ProxyID, float> _floats = {};
            static inline std::unordered_map<ProxyID, bool> _bools = {};

            static inline std::unordered_map<ProxyID, std::array<float, 2>> _vec2s = {};
            static inline std::unordered_map<ProxyID, std::array<float, 3>> _vec3s = {};
            static inline std::unordered_map<ProxyID, std::array<float, 4>> _vec4s = {};

            static inline std::unordered_map<ProxyID, std::vector<float>> _array_vec1s = {};
            static inline std::unordered_map<ProxyID, std::vector<std::array<float, 2>>> _array_vec2s = {};
            static inline std::unordered_map<ProxyID, std::vector<std::array<float, 3>>> _array_vec3s = {};
            static inline std::unordered_map<ProxyID, std::vector<std::array<float, 4>>> _array_vec4s = {};

            // shaders
            static void initialize_vertex_shader();
            static inline GLNativeHandle _vertex_shader = -1;
            static inline std::multiset<GLNativeHandle> _fragment_shaders = {};
            static inline std::multiset<GLNativeHandle> _shader_programs = {};

            static inline crisp::Shader* _active_shader = nullptr;
            static inline GLNativeHandle _active_program = -1;

            // textures
            static inline bool _vertices_initialized = false;
            static inline GLNativeHandle _vertex_array = -1,
                                          _vertex_buffer = -1,
                                          _element_buffer = -1;

            struct TextureInfo
            {
                PaddingType padding_type;
                size_t width,
                       height;
            };

            static inline std::multiset<GLNativeHandle> _textures = {};
            static inline std::unordered_map<GLNativeHandle, TextureInfo> _texture_info;

            static inline GLNativeHandle _active_texture = 0;
    };
}

#include ".src/state.inl"