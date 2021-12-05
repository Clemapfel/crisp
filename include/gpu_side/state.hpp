// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <set>

#include <SFML/Graphics/Glsl.hpp>
#include <GLES3/gl3.h>

#include <SFML/Window.hpp>

#include <vector.hpp>
#include <padding_type.hpp>
#include <image/multi_plane_image.hpp>
#include <gpu_side/native_handle.hpp>
#include <structuring_element.hpp>

namespace crisp
{
    class Shader;

    /// @brief resource id of internal shaders
    using ShaderID = std::string;

    /// @brief handler that manages resource allocation and interaction between cpu-side and gpu-side resources
    /// @note do not interact with this class unless you know what you are doing, intended for internal use only
    union State
    {
        friend class Shader;
        friend class Workspace;

        public:
            /// @brief ctor disabled, all members and member functions are static
            State() = delete;

            /// @brief clear the current pixel buffer and render the currently bound shader program
            static void display();

            /// @brief create an image from the current pixel buffer state, similar to a screenshot
            /// @param image: [out] image the result will be written to, image size governs resulting screenshot size
            /// @param top_left: [in] top-left alignment
            template<typename T>
            static void get_pixel_buffer(Image<T, 1>& image, Vector2ui top_left = {0, 0});

            /// @brief create an image from the current pixel buffer state, similar to a screenshot
            /// @param image: [out] image the result will be written to, image size governs resulting screenshot size
            /// @param top_left: [in] top-left alignment
            template<typename T>
            static void get_pixel_buffer(Image<T, 2>& image, Vector2ui top_left = {0, 0});

            /// @brief create an image from the current pixel buffer state, similar to a screenshot
            /// @param image: [out] image the result will be written to, image size governs resulting screenshot size
            /// @param top_left: [in] top-left alignment
            template<typename T>
            static void get_pixel_buffer(Image<T, 3>& image, Vector2ui top_left = {0, 0});

            /// @brief create an image from the current pixel buffer state, similar to a screenshot
            /// @param image: [out] image the result will be written to, image size governs resulting screenshot size
            /// @param top_left: [in] top-left alignment
            template<typename T>
            static void get_pixel_buffer(Image<T, 4>& image, Vector2ui top_left = {0, 0});

            /// @brief allocate integer
            /// @param value
            /// @returns id of allocated resource
            static ProxyID register_int(int);
            
            /// @brief deallocate integer
            /// @param resource_id
            static void free_int(ProxyID);
            
            /// @brief bind already allocated integer to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_int(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief bind free integer to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param int
            static void set_int(GLNativeHandle program_id, const std::string& var_name, int);

            /// @brief allocate unsigned integer
            /// @param value
            /// @returns id of allocated resource
            static ProxyID register_uint(size_t);

            /// @brief deallocate unsigned integer
            /// @param resource_id
            static void free_uint(ProxyID);

            /// @brief bind already allocated unsigned integer to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_uint(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief bind free  unsigned integer to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param int
            static void set_uint(GLNativeHandle program_id, const std::string& var_name, int);

            /// @brief allocate float
            /// @param value
            /// @returns id of allocated resource
            static ProxyID register_float(float);
            
            /// @brief deallocate float
            /// @param resource_id
            static void free_float(ProxyID);

            /// @brief bind already allocated float to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_float(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief bind free float to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param float
            static void set_float(GLNativeHandle program_id, const std::string& var_name, float);

            /// @brief allocate bool
            /// @param value
            /// @returns id of allocated resource
            static ProxyID register_bool(bool);
            
            /// @brief deallocate bool
            /// @param resource_id
            static void free_bool(ProxyID);

            /// @brief bind already allocated bool to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_bool(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief bind free bool to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param bool
            static void set_bool(GLNativeHandle program_id, const std::string& var_name, bool);

            /// @brief register vector of size 2, 3 or 4
            /// @tparam N: size of vector
            /// @tparam T: value type of vector, will be static cast to float
            /// @param vector
            /// @returns resulting handle
            template<size_t N, typename T>
            [[nodiscard]] static ProxyID register_vec(const Vector<T, N>&);

            /// @brief free vector of any size
            /// @param vector id
            static void free_vec(ProxyID);

            /// @brief bind vector of any size
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param id of vector
            static void bind_vec(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief directly bind a vector, is free once a different program is bound
            /// @tparam N: size of vector, in {2, 3, 4}
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param vector
            template<size_t N>
            static void set_vec(GLNativeHandle program_id, const std::string& var_name, Vector<float, N>);

            /// @brief allocate array of floats
            /// @param vector
            /// @returns id of allocated resource
            template<typename T>
            [[nodiscard]] static ProxyID register_array(const std::vector<T>&);

            /// @brief allocate array of floats
            /// @param array
            /// @returns id of allocated resource
            template<typename T, size_t N>
            [[nodiscard]] static ProxyID register_array(const std::array<T, N>&);

            /// @brief allocate array of vec2s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            [[nodiscard]] static ProxyID register_vec2_array(const std::vector<crisp::Vector<T, 2>>&);

            /// @brief allocate array of vec3s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            [[nodiscard]] static ProxyID register_vec3_array(const std::vector<crisp::Vector<T, 3>>&);

            /// @brief allocate array of vec4s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            [[nodiscard]] static ProxyID register_vec4_array(const std::vector<crisp::Vector<T, 4>>&);

            /// @brief deallocate array of vectors
            /// @tparam N: 1 for array for floats, 2 for array of vec2s, 3 for vec3s, 4 for vec4s
            /// @param resource_id
            template<size_t N>
            static void free_array(ProxyID);

            /// @brief bind already allocated array to uniform of shader program
            /// @tparam N: 1 for array for floats, 2 for array of vec2s, 3 for vec3s, 4 for vec4s
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            template<size_t N>
            static void bind_array(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief allocate matrix of size nxm where n in {2, 3, 4} and m in {2, 3, 4}
            /// @tparam T: value type of matrix
            /// @tparam n_rows: number of rows, in {2, 3, 4}
            /// @tparam n_cols: number of cols, in {2, 3, 4}
            /// @param value
            /// @returns id: id of allocated resource
            template<typename T>
            [[nodiscard]] static ProxyID register_matrix(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&);

            /// @brief allocated structuring element as matrix
            /// @param se: structuring elements
            /// @returns id of allocated matrix
            /// @note a "not care" value will be internally treated as -1.f
            [[nodiscard]] static ProxyID register_structuring_element(const StructuringElement& se);

            /// @brief allocated non-flat structuring element as matrix
            /// @param se: non-flat structuring elements
            /// @returns id of allocated matrix
            /// @note a "not care" value will be internally treated as -1.f
            [[nodiscard]] static ProxyID register_structuring_element(const NonFlatStructuringElement& se);

            /// @brief deallocate matrix of any size
            /// @param resource_id
            static void free_matrix(ProxyID);

            /// @brief bind already allocated vec4 to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_matrix(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);
            
            /// @brief load shader, compile it and allocated gpu-side
            /// @param id: internal id
            /// @returns native handle of shader
            [[nodiscard]] static GLNativeHandle register_shader(const ShaderID& id);

            /// @brief load shader, takes absolute path instead of internal id
            /// @param path: absolute path
            /// @returns native handle of shader
            [[nodiscard]] static GLNativeHandle register_shader_from_source(std::string);
            
            /// @brief deallocate shader
            /// @param id: native handle of shader
            static void free_shader(GLNativeHandle);
            
            /// @brief bind shader to shader program, allocate it gpu-side
            /// @param path: absolute path to shader source code
            /// @returns native handle of shader program
            [[nodiscard]]  static GLNativeHandle register_program(GLNativeHandle fragment_shader_id);

            /// @brief deallocate shader program
            /// @param id: native handle of program
            static void free_program(GLNativeHandle);

            /// @brief set already allocated shader program as active
            /// @param program_id: native handle of program
            static void bind_shader_program(GLNativeHandle program_id);

            /// @brief create texture from image, then allocate it gpu-side
            /// @param image: image to be converted
            /// @returns native handle of texture
            template<typename T, size_t N>
            [[nodiscard]] static GLNativeHandle register_texture(const Image<T, N>&);

            /// @brief create an empty texture of specified size gpu-side
            /// @param width: x-dimension of the texture
            /// @param height: y-dimension of the texture
            /// @returns native handle of texture
            template<typename T, size_t N>
            [[nodiscard]] static GLNativeHandle register_texture(size_t width, size_t height);

            /// @brief create a deep-copy of an already existing texture
            /// @param to_copy: handle of already existing texture
            /// @returns native handle of resulting texture
            template<typename T, size_t N>
            [[nodiscard]] static GLNativeHandle register_texture(GLNativeHandle to_copy);

            /// @brief create texture from raw data
            /// @param data: OpenGL compliant formatted array
            /// @returns native handle of resulting texture
            template<typename T, size_t N>
            [[nodiscard]] static GLNativeHandle register_texture(size_t width, size_t height, const std::vector<T>& data);

            /// @brief create texture from raw data
            /// @param data: OpenGL compliant formatted array
            /// @returns native handle of resulting texture
            template<typename T, size_t N>
            [[nodiscard]] static GLNativeHandle register_texture(size_t width, size_t height, const T* data);

            /// @brief deallocate texture
            /// @param id: native handle of texture
            static void free_texture(GLNativeHandle);

            /// @brief bind already allocated texture to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param texture_id: native handle of texture
            /// @param texture_location: value of layout qualifier
            /// @notes when binding multiple textures to the same program, start with the texture with the *highest* texture unit first, then proceed towards the lowest
            static void bind_texture(GLNativeHandle program_id, const std::string& var_name, GLNativeHandle texture_id, size_t texture_location = 0);

            /// @brief return size of texture already allocated
            /// @param texture_handle
            /// @returns size as vector
            static Vector2ui get_texture_size(GLNativeHandle);

            /// @brief download texture data from the gpu and return it
            /// @returns raw data of texture
            [[nodiscard]] static std::vector<float> get_texture_data(GLNativeHandle);

            /// @brief object that holds information about registered textures
            struct TextureInfo
            {
                /// @brief type of padding
                PaddingType padding_type;

                /// @brief x-dimension of texture
                size_t width;

                /// @brief y-dimension of texture
                size_t height;

                /// @brief number of planes
                size_t n_planes;

                /// @brief gl-type, one of GL_RED, GL_RG, GL_RGB, GL_RGBA
                GLenum type;
            };

            /// @brief get stored texture info
            /// @param texture_handle
            /// @returns texture info as State::TextureInfo object
            static TextureInfo get_texture_info(GLNativeHandle);

            /// @brief register 1d signal of arbitrary length
            /// @param n_samples: number of elements in data
            /// @param data: pointer to data, 16-bit int
            /// @returns native handle of GL_TEXTURE_1D_ARRAY
            static GLNativeHandle register_1d_signal(size_t n_samples, size_t first_sample, const float* data);

            /// @brief free 1d signal
            /// @param handle
            static GLNativeHandle free_1d_signal(GLNativeHandle);

            /// @brief bind 1d signal
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param signal_id: native handle of signal
            /// @param texture_location: value of layout qualifier
            /// @notes when binding multiple textures to the same program, start with the texture with the *highest* texture unit first, then proceed towards the lowest
            static void bind_1d_signal(GLNativeHandle program_id, const std::string& var_name, GLNativeHandle signal_id, size_t texture_location = 0);

            /// @brief get id of currently bound shader program
            /// @returns -1 if no programs bound, the programs native handle in {1, 2, ...} otherwise
            static GLNativeHandle get_active_program_handle();

            /// @brief set the internal context as the active context
            void set_internal_context_active();

        private:
            // context
            static inline sf::ContextSettings context_settings = sf::ContextSettings(
                    0,
                    0,
                    0,
                    3,
                    3,
                    sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Default);
            static inline sf::Context _context = sf::Context(context_settings, 1, 1);

            // proxies
            static inline ProxyID _current = -1;
            static ProxyID get_next_id() { return _current--;}

            static inline std::unordered_map<ProxyID, int> _ints = {};
            static inline std::unordered_map<ProxyID, unsigned int> _uints = {};
            static inline std::unordered_map<ProxyID, float> _floats = {};
            static inline std::unordered_map<ProxyID, bool> _bools = {};

            static inline std::unordered_map<ProxyID, std::vector<float>> _vecs = {};

            static inline std::unordered_map<ProxyID, std::vector<float>> _array_vec1s = {};
            static inline std::unordered_map<ProxyID, std::vector<float>> _array_vec2s = {};
            static inline std::unordered_map<ProxyID, std::vector<float>> _array_vec3s = {};
            static inline std::unordered_map<ProxyID, std::vector<float>> _array_vec4s = {};

            struct MatrixProxy
            {
                std::vector<float> data;
                size_t n_rows,
                       n_cols;
            };

            static inline std::unordered_map<ProxyID, MatrixProxy> _mats = {};

            static void initialize_noop_shaders();
            static inline GLNativeHandle _noop_vertex_shader = NONE;
            static inline GLNativeHandle _noop_fragment_shader = NONE;
            static inline GLNativeHandle _noop_program = NONE;
            static inline bool _noop_initialized = false;

            static inline std::multiset<GLNativeHandle> _fragment_shaders = {};
            static inline std::multiset<GLNativeHandle> _shader_programs = {};

            static inline GLNativeHandle _active_program = NONE;

            static void initialize_vertices();
            static inline bool _vertices_initialized = false;
            static inline GLNativeHandle _vertex_array = NONE,
                                         _vertex_buffer = NONE,
                                         _element_buffer = NONE;

            static inline std::multiset<GLNativeHandle> _textures = {};
            static inline std::unordered_map<GLNativeHandle, TextureInfo> _texture_info = {};

            struct FrameBufferProxy
            {
                GLNativeHandle buffer_handle,
                               texture_handle;
                size_t width,
                       height;
            };

            static inline std::unordered_map<GLNativeHandle, FrameBufferProxy> _frame_buffer = {};
            static inline GLNativeHandle _active_buffer = NONE;

            static inline std::multiset<GLNativeHandle> _1d_texture_arrays = {};

            // exceptions and safety
            static void verify_program_id(GLNativeHandle);
    };
}

#include ".src/state.inl"