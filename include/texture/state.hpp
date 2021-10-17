// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <set>

#include <vector.hpp>
#include <padding_type.hpp>
#include <texture/native_handle.hpp>
#include <texture/texture.hpp>

namespace crisp
{

    struct TextureProxy
    {
        GLNativeHandle _native_handle;
        PaddingType _padding_type;

        unsigned int vertex_array_id,
                     vertex_buffer_id,
                     element_buffer_id;
    };

    using ProxyID = int;    // -1, -2, ... so it isn't confused with GLNativeHandle

    template<typename T>
    struct Proxy
    {
        Proxy(const T& value);
        ProxyID get_id() const;

        private:
            ProxyID id;
    };

    template<typename T>
    using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

    // holds proxies to all objects relevant for currently registered shaders
    struct State
    {
        static ProxyID register_int(int);
        static void free_int(ProxyID);

        static ProxyID register_float(float);
        static void free_float(ProxyID);

        static ProxyID register_bool(bool);
        static void free_bool(ProxyID);

        template<typename T>
        static ProxyID register_vec2(const Vector<T, 2>&);

        static void free_vec2(ProxyID);

        template<typename T>
        static ProxyID register_vec3(const Vector<T, 3>&);

        static void free_vec3(ProxyID);

        template<typename T>
        static ProxyID register_vec4(const Vector<T, 4>&);

        static void free_vec4(ProxyID);

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

        /// @param path: [in]
        /// @param program_id: [out]
        /// @param shader_id: [out]
        static GLNativeHandle register_shader(const std::string& path);
        static void free_shader(GLNativeHandle);

        static GLNativeHandle register_program(GLNativeHandle fragment_shader_id);
        static void free_program(GLNativeHandle);

        template<typename T, size_t N>
        static GLNativeHandle register_texture(const Texture<T, N>&);
        void free_texture(GLNativeHandle);

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

            // textures
            static inline std::vector<GLNativeHandle> _textures = {};
    };
}

#include ".src/state.inl"