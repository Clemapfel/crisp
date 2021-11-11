// 
// Copyright 2021 Clemens Cords
// Created on 09.11.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>
#include <gpu_side/texture.hpp>

namespace crisp
{
    /// @brief matrix class that is operated upon by the cpu
    class HardwareAcceleratedMatrix
    {
        public:
            HardwareAcceleratedMatrix(size_t n_rows, size_t n_cols, float value = 0);
            explicit HardwareAcceleratedMatrix(const Eigen::MatrixXf&);

            /// @brief copy ctor
            HardwareAcceleratedMatrix(const HardwareAcceleratedMatrix&);

            /// @brief move ctor
            HardwareAcceleratedMatrix(HardwareAcceleratedMatrix&&) = default;

            /// @brief create matrix by concatenating given matrix horizontally n times
            void create_from_concatenate_horizontal(const HardwareAcceleratedMatrix& matrix, size_t n);
            void create_from_concatenate_vertical(const HardwareAcceleratedMatrix& matrix, size_t n);

            /// @brief download data to ram
            [[nodiscard]] std::vector<float> get_data() const;

            size_t rows() const;
            size_t cols() const;

            /// @brief copy assignment
            HardwareAcceleratedMatrix& operator=(const HardwareAcceleratedMatrix&);

            /// @brief move assignemtn
            HardwareAcceleratedMatrix& operator=(HardwareAcceleratedMatrix&&) = default;

            /// @brief assign constant to all elements
            void set_constant(float);

            /// @brief transpose
            HardwareAcceleratedMatrix& transpose();

            /// @brief matrix-matrix multiplication
            HardwareAcceleratedMatrix& operator^(const HardwareAcceleratedMatrix&);

            /// @brief elementwise add
            HardwareAcceleratedMatrix& operator+(const HardwareAcceleratedMatrix&);
            HardwareAcceleratedMatrix& operator+=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise multiply
            HardwareAcceleratedMatrix& operator*(const HardwareAcceleratedMatrix&);
            HardwareAcceleratedMatrix& operator*=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise subtract
            HardwareAcceleratedMatrix& operator-(const HardwareAcceleratedMatrix&);
            HardwareAcceleratedMatrix& operator-=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise divide
            HardwareAcceleratedMatrix& operator/(const HardwareAcceleratedMatrix&);
            HardwareAcceleratedMatrix& operator/=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise scalar add
            HardwareAcceleratedMatrix& operator+(float scalar);
            HardwareAcceleratedMatrix& operator+=(float scalar);

            /// @brief elementwise scalar multiply
            HardwareAcceleratedMatrix& operator*(float scalar);
            HardwareAcceleratedMatrix& operator*=(float scalar);

            /// @brief elementwise scalar subtract
            HardwareAcceleratedMatrix& operator-(float scalar);
            HardwareAcceleratedMatrix& operator-=(float scalar);

            /// @brief elementwise scalar divide
            HardwareAcceleratedMatrix& operator/(float scalar);
            HardwareAcceleratedMatrix& operator/=(float scalar);

        private:
            void init();

            GLNativeHandle _buffer;

            // shader program handles
            static inline GLNativeHandle _product = -1;
            static inline GLNativeHandle _ewise_sum = -1, _ewise_product = -1;
            static inline GLNativeHandle _scalar_sum = -1, _scalar_product = -1;
            static inline GLNativeHandle _transpose = -1;

            size_t _n_rows, _n_cols;


            Texture<float, 1> _texture;
            Texture<float, 1> _buffer_texture;
    };
}

#include ".src/hardware_accelerated_matrix.inl"