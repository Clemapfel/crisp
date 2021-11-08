// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    /// @brief Vector class that behaves like a scalar, all arithmetics are element-wise
    /// @param T: value type, must be arithmetic
    /// @param N: number of components
    template<typename T, size_t N>
    class Vector : public Eigen::Array<T, 1, N>
    {
        static_assert(std::is_arithmetic<T>::value);

        public:
            /// @brief expose T as member typedef
            using Value_t = T;

            /// @brief get number of components
            /// @returns N
            static constexpr size_t size() noexcept;

            /// @brief default ctor
            Vector() noexcept;

            /// @brief construct variadic
            /// @param args: initializer list of length N
            Vector(std::initializer_list<T>);

            /// @brief ctor and assign all elements
            /// @param T: value
            /// @note this ctor is only implicit for N = 1
            explicit (N != 1) Vector(T);

            /// @brief decays vector to element for N = 1, throws static assertion if N > 1
            explicit (N != 1) operator T() const;

            /// @brief cast vector to different value type
            /// @tparam NewT: new numerical value type
            template<typename NewT>
            Vector<NewT, N> cast_to() const;

            /// @brief access data without bounds checking
            /// @param index
            /// @returns reference to component
            T& operator[](size_t i) noexcept;

            /// @brief const-access data without bounds checking
            /// @param index
            /// @returns copy of component
            T operator[](size_t i) const noexcept;

            /// @brief access data with bounds checking
            /// @param index: in [0, N-1]
            /// @returns reference to component
            T& at(size_t i);

            /// @brief const-access data with bounds checking
            /// @param index: in [0, N-1]
            /// @returns copy of component
            T at(size_t i) const;

            /// @brief translate to hash, hash collisions will appear if any component is outside of [0,1]
            size_t to_hash() const;

            /// @brief vector-vector element-wise addition
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator+(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise subtraction
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator-(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise multiplication
            /// @param other: vector of same size
            /// @returns resulting vector
            using Eigen::Array<T, 1, N>::operator*;
            
            /// @brief vector-vector element-wise division
            /// @param other: vector of same size
            /// @returns resulting vector
            using Eigen::Array<T, 1, N>::operator/;

            /// @brief vector-vector element-wise assignment addition
            /// @param other: vector of same size
            /// @returns reference to self after assignment
            using Eigen::Array<T, 1, N>::operator+=;
            
            /// @brief vector-vector element-wise assignment subtraction
            /// @param other: vector of same size
            /// @returns reference to self after assignment
            using Eigen::Array<T, 1, N>::operator-=;
            
            /// @brief vector-vector element-wise assignment multiplication
            /// @param other: vector of same size
            /// @returns reference to self after assignment
            using Eigen::Array<T, 1, N>::operator*=;
            
            /// @brief vector-vector element-wise assignment division
            /// @param other: vector of same size
            /// @returns reference to self after assignment
            using Eigen::Array<T, 1, N>::operator/=;
            
            /// @brief vector-vector element-wise modulo assignment
            /// @param other: vector of same size
            /// @returns reference to self after assignment
            Vector<T, N>& operator%=(const Vector<T, N>& other) noexcept;

            /// @brief vector-vector element-wise, bit-wise AND
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator&(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise, short-circuit AND
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator&&(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise, bit-wise OR
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator|(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise, short-circuit OR
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator||(const Vector<T, N>& other) const noexcept;
            
            /// @brief vector-vector element-wise, bit-wise XOR
            /// @param other: vector of same size
            /// @returns resulting vector
            Vector<T, N> operator^(const Vector<T, N>& other) const noexcept;
            
            /// @brief assigns scalar to all components
            Vector<T, N>& operator=(T) noexcept;

            /// @brief vector-scalar element-wise arithmetic addition
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator+(T) const noexcept;
            
            /// @brief vector-scalar element-wise arithmetic subtraction
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator-(T) const noexcept;
            
            /// @brief vector-scalar element-wise arithmetic multiplication
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator*(T) const noexcept;
            
            /// @brief vector-scalar element-wise arithmetic division
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator/(T) const noexcept;
            
            /// @brief vector-scalar element-wise arithmetic modulo
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator%(T) const noexcept;

            /// @brief vector-scalar element-wise, bit-wise AND
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator&(T) const noexcept;
            
            /// @brief vector-scalar element-wise, short-circuit AND
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator&&(T) const noexcept;
            
            /// @brief vector-scalar element-wise, bit-wise OR
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator|(T) const noexcept;

            /// @brief vector-scalar element-wise, short-circuit OR
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator||(T) const noexcept;

            /// @brief vector-scalar element-wise, bit-wise XOR
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N> operator^(T) const noexcept;

            /// @brief vector-scalar element-wise, bit-wise assignment AND
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N>& operator&=(T) noexcept;

            /// @brief vector-scalar element-wise, bit-wise assignment OR
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N>& operator|=(T) noexcept;

            /// @brief vector-scalar element-wise, bit-wise assignment XOR
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N>& operator^=(T) noexcept;

            /// @brief vector-scalar element-wise assignment modulo
            /// @param scalar
            /// @returns resulting vector
            Vector<T, N>& operator%=(T) noexcept;

            /// @brief unary bitwise NOT
            /// @returns resulting vector
            Vector<T, N> operator~() const noexcept;

            /// @brief unary boolean not
            /// @returns resulting vector
            using Eigen::Array<T, 1, N>::operator!;

            /// @brief vector-vector element-wise equality
            /// @param other: vector of same size
            /// @return true if all respective components are equal, false otherwise
            bool operator==(const Vector<T, N>& other) const noexcept;

            /// @brief vector-vector element-wise equality
            /// @param other: vector of same size
            /// @return false if all respective components are equal, true otherwise
            bool operator!=(const Vector<T, N>& other) const noexcept;

            /// @brief vector-vector element-wise less than
            /// @param other: vector of same size
            /// @returns true if hash of self is smaller than hash of other
            bool operator<(const Vector<T, N>&) const noexcept;

            /// @brief vector-vector element-wise less than or equal
            /// @param other: vector of same size
            /// @returns true equal or if hash of self is smaller than hash of other
            bool operator<=(const Vector<T, N>&) const noexcept;

            /// @brief vector-vector element-wise greater than
            /// @param other: vector of same size
            /// @returns true if hash of self is greater than hash of other
            bool operator>(const Vector<T, N>&) const noexcept;

            /// @brief vector-vector element-wise greater than or equal
            /// @param other: vector of same size
            /// @returns true if equal or if hash of self is greater than hash of other
            bool operator>=(const Vector<T, N>&) const noexcept;

            /// @brief vector-scalar element-wise equality
            /// @param scalar
            /// @returns true if all components are equal to scalar, false otherwise
            bool operator==(T) const noexcept;

            /// @brief vector-scalar element-wise equality
            /// @param scalar
            /// @returns false if all components are equal to scalar, true otherwise
            bool operator!=(T) const noexcept;

            /// @brief vector-scalar element-wise less than
            /// @param scalar
            /// @returns true if all components are less than scalar, false otherwise
            bool operator<(T) const noexcept;

            /// @brief vector-scalar element-wise less than
            /// @param scalar
            /// @returns true if all components are less than or equal to scalar, false otherwise
            bool operator<=(T) const noexcept;

            /// @brief vector-scalar element-wise less than
            /// @param scalar
            /// @returns true if all components are greater than scalar, false otherwise
            bool operator>(T) const noexcept;

            /// @brief vector-scalar element-wise less than
            /// @param scalar
            /// @returns true if all components are greater than or equal to scalar, false otherwise
            bool operator>=(T) const noexcept;

            /// @brief access first component
            /// @returns reference to first component
            using Eigen::Array<T, 1, N>::x;

            /// @brief access second component
            /// @returns reference to second component
            using Eigen::Array<T, 1, N>::y;

            /// @brief access third component
            /// @returns reference to third component
            using Eigen::Array<T, 1, N>::z;

            /// @brief access third component
            /// @returns reference to third component
            using Eigen::Array<T, 1, N>::w;
    };

    using Vector2f = Vector<float, 2>;
    using Vector2i = Vector<int, 2>;
    using Vector2ui = Vector<size_t, 2>;

    using Vector3f = Vector<float, 3>;
    using Vector3i = Vector<int, 3>;
    using Vector3ui = Vector<size_t, 3>;

    using Vector4f = Vector<float, 4>;
    using Vector4i = Vector<int, 4>;
    using Vector4ui = Vector<size_t, 4>;
}

/// @brief hash vector by first mapping it to a unique string, then hashing that string
template<typename T, size_t N>
struct std::hash<crisp::Vector<T, N>>;

#include ".src/vector.inl"