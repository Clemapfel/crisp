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
            /// @brief construct set constant
            /// @param n_rows: x-dimension
            /// @param n_cols: y-dimension
            /// @param value: constant
            HardwareAcceleratedMatrix(size_t n_rows, size_t n_cols, float value = 0);

            /// @brief construct from eigen matrix
            /// @param matrix: dynamic eigen matrix
            explicit HardwareAcceleratedMatrix(const Eigen::MatrixXf&);

            /// @brief construct matrix from already allocated texture
            /// @param texture
            explicit HardwareAcceleratedMatrix(GLNativeHandle texture);

            /// @brief copy ctor
            /// @param reference to const
            HardwareAcceleratedMatrix(const HardwareAcceleratedMatrix&);

            /// @brief move ctor
            /// @param rvalue
            HardwareAcceleratedMatrix(HardwareAcceleratedMatrix&&) = default;

            /// @brief create matrix by concatenating given matrix horizontally n times
            /// @param matrix: sub-matrix A
            /// @param n: number of concatenation
            /// @returns Matrix in the form A | A | A | ... | A n-times
            void create_from_concatenate_horizontal(const HardwareAcceleratedMatrix& matrix, size_t n);

            /// @brief create matrix by concatenating given matrix horizontally n times
            /// @param matrix: sub-matrix A
            /// @param n: number of concatenation
            /// @returns Matrix
            void create_from_concatenate_vertical(const HardwareAcceleratedMatrix& matrix, size_t n);

            /// @brief download data to ram
            /// @returns vector with data in row-major order
            [[nodiscard]] std::vector<float> get_data() const;

            /// @brief get x-dimension
            /// @returns size_t
            size_t rows() const;

            /// @brief get y-dimension
            /// @returns size_t
            size_t cols() const;

            /// @brief copy assignment
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator=(const HardwareAcceleratedMatrix&);

            /// @brief move assignment
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator=(HardwareAcceleratedMatrix&&) noexcept;

            /// @brief assign constant to all elements
            /// @param constant
            void set_constant(float);

            /// @brief transpose, invokes copy ctor
            /// @returns transposed matrix by value
            HardwareAcceleratedMatrix transpose() const;

            /// @brief transpose and assign to self, does not invoke copy ctor
            void transpose_in_place();

            /// @brief matrix-matrix multiplication, invokes copy ctor
            /// @param other
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator^(const HardwareAcceleratedMatrix&) const;

            /// @brief matrix-matrix multiplication, assigns to self without invoking copy ctor
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator^=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise add, invokes copy ctor
            /// @param other
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator+(const HardwareAcceleratedMatrix&) const;

            /// @brief elementwise add, assigns to self without invoking copy ctor
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator+=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise multiply, invokes copy ctor
            /// @param other
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator*(const HardwareAcceleratedMatrix&) const;

            /// @brief elementwise multiply, assigns to self without invoking copy ctor
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator*=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise subtract, invokes copy ctor
            /// @param other
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator-(const HardwareAcceleratedMatrix&) const;

            /// @brief elementwise subtract, assigns to self without invoking copy ctor
            /// @param other
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator-=(const HardwareAcceleratedMatrix&);

            /// @brief elementwise scalar add, invokes copy ctor
            /// @param scalar
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator+(float scalar) const;

            /// @brief elementwise scalar add, assigns to self without invoking copy ctor
            /// @param scalar
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator+=(float scalar);

            /// @brief elementwise scalar multiply, invokes copy ctor
            /// @param scalar
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator*(float scalar) const;

            /// @brief elementwise scalar multiply, assigns to self without invoking copy ctor
            /// @param scalar
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator*=(float scalar);

            /// @brief elementwise scalar subtract, invokes copy ctor
            /// @param scalar
            /// @returns new matrix by value
            HardwareAcceleratedMatrix operator-(float scalar) const;

            /// @brief elementwise scalar subtract, assigns to self without invoking copy ctor
            /// @param scalar
            /// @returns reference to self
            HardwareAcceleratedMatrix& operator-=(float scalar);

        protected:
            HardwareAcceleratedMatrix() = default;

            /// @brief elementwise scalar divide
            [[deprecated]] HardwareAcceleratedMatrix operator/(float scalar) const;
            [[deprecated]] HardwareAcceleratedMatrix& operator/=(float scalar);

            /// @brief elementwise divide
            HardwareAcceleratedMatrix operator/(const HardwareAcceleratedMatrix&) const;
            HardwareAcceleratedMatrix& operator/=(const HardwareAcceleratedMatrix&);

        private:
            void init();

            GLNativeHandle _buffer;

            // shader program handles
            static inline GLNativeHandle _product = -1;
            static inline GLNativeHandle _ewise_sum = -1, _ewise_product = -1, _ewise_divide = -1;
            static inline GLNativeHandle _scalar_sum = -1, _scalar_product = -1;
            static inline GLNativeHandle _transpose = -1;

            size_t _n_rows, _n_cols;


            Texture<float, 1> _texture;
            mutable Texture<float, 1> _buffer_texture;
    };
}

#include ".src/hardware_accelerated_matrix.inl"