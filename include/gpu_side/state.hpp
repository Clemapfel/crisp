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
#include <image/multi_plane_image.hpp>
#include <gpu_side/native_handle.hpp>

#define SHADER_PATH "/home/clem/Workspace/crisp/include/gpu_side/.shaders/"

namespace crisp
{
    class Shader;
    using ShaderID = std::string;

    /// @brief handler that manages resource allocation and interaction between cpu-side and gpu-side resources
    class State
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

        //protected:
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

            /// @brief allocate vec2
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec2(const Vector<T, 2>&);
            
            /// @brief deallocate vec2
            /// @param resource_id
            static void free_vec2(ProxyID);

            /// @brief bind already allocated vec2 to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_vec2(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief allocate vec3
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec3(const Vector<T, 3>&);
            
            /// @brief deallocate vec3
            /// @param resource_id
            static void free_vec3(ProxyID);

            /// @brief bind already allocated vec3 to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_vec3(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief allocate vec3
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec4(const Vector<T, 4>&);
            
            /// @brief deallocate vec4
            /// @param resource_id
            static void free_vec4(ProxyID);

            /// @brief bind already allocated vec4 to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_vec4(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);

            /// @brief allocate array of floats
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_array(const std::vector<T>&);

            /// @brief allocate array of vec2s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec2_array(const std::vector<crisp::Vector<T, 2>>&);

            /// @brief allocate array of vec3s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec3_array(const std::vector<crisp::Vector<T, 3>>&);

            /// @brief allocate array of vec4s
            /// @param value
            /// @returns id of allocated resource
            template<typename T>
            static ProxyID register_vec4_array(const std::vector<crisp::Vector<T, 4>>&);

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
            template<typename T, size_t n_rows, size_t n_cols>
            static ProxyID register_matrix(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&);
            
            /// @brief deallocate matrix of any size
            /// @param resource_id
            static void free_matrix(ProxyID);

            /// @brief bind already allocated vec4 to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param proxy_id: id of resource
            static void bind_matrix(GLNativeHandle program_id, const std::string& var_name, ProxyID proxy_id);
            
            /// @brief load shader, compile it and allocated gpu-side
            /// @param path: absolute path to shader source code
            /// @returns native handle of shader
            static GLNativeHandle register_shader(const ShaderID& id);
            
            /// @brief deallocate shader
            /// @param id: native handle of shader
            static void free_shader(GLNativeHandle);
            
            /// @brief bind shader to shader program, allocate it gpu-side
            /// @param path: absolute path to shader source code
            /// @returns native handle of shader program
            static GLNativeHandle register_program(GLNativeHandle fragment_shader_id);

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
            static GLNativeHandle register_texture(const Image<T, N>&);

            template<typename T, size_t N>
            static GLNativeHandle register_texture(size_t width, size_t height);

            /// @brief deallocate texture
            /// @param id: native handle of texture
            static void free_texture(GLNativeHandle);

            /// @brief bind already allocated texture to uniform of shader program
            /// @param program_id: native handle of shader program
            /// @param var_name: exact variable name in shader source
            /// @param texture_id: native handle of texture
            /// @param texture_location: value of layout qualifier
            static void bind_texture(GLNativeHandle program_id, const std::string& var_name, GLNativeHandle texture_id, size_t texture_location = 0);

            static GLNativeHandle register_framebuffer(GLNativeHandle texture_handle);
            static void free_framebuffer(GLNativeHandle buffer_handle);
            static void bind_framebuffer(GLNativeHandle buffer_handle);
            static void copy_framebuffer_to_texture(GLNativeHandle buffer_handle, GLNativeHandle texture_handle);

            static GLNativeHandle get_active_program_handle();

        private:
            static inline ProxyID _current = 0;
            static ProxyID get_next_id() { return _current--;}

            static inline std::unordered_map<ProxyID, int> _ints = {};
            static inline std::unordered_map<ProxyID, float> _floats = {};
            static inline std::unordered_map<ProxyID, bool> _bools = {};

            static inline std::unordered_map<ProxyID, std::array<float, 2>> _vec2s = {};
            static inline std::unordered_map<ProxyID, std::array<float, 3>> _vec3s = {};
            static inline std::unordered_map<ProxyID, std::array<float, 4>> _vec4s = {};

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

            static inline std::multiset<GLNativeHandle> _fragment_shaders = {};
            static inline std::multiset<GLNativeHandle> _shader_programs = {};

            static inline GLNativeHandle _active_program = NONE;

            static inline bool _vertices_initialized = false;
            static inline GLNativeHandle _vertex_array = NONE,
                                         _vertex_buffer = NONE,
                                         _element_buffer = NONE;

            struct TextureInfo
            {
                PaddingType padding_type;
                size_t width,
                       height;
            };

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

            // exceptions and safety
            static void verify_program_id(GLNativeHandle);
    };
}

#include ".src/state.inl"