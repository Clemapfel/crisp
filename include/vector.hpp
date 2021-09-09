// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    template<typename, size_t>
    class Vector;
}

namespace crisp
{
    // Vector class that behaves like a scalar, all arithmetics are element-wise
    template<typename T, size_t N>
    class Vector : public Eigen::Array<T, 1, N>
    {
        static_assert(std::is_integral<T>::value or std::is_floating_point<T>::value);

        public:
            using Value_t = T;

            // @brief default ctor
            Vector();

            // @brief ctor and assign all elements
            explicit Vector(T);

            // @brief construct variadic
            // @note if the number of arguments exceeds N, the behavior is undefined
            Vector(std::initializer_list<T>);

            // @brief decays vector to element for N = 1, casts to norm otherwise
            explicit (N != 1) operator T() const;

            // @brief access data without bounds checking
            // @param i: index
            // @returns: reference to element
            T& operator[](size_t i);

            // @brief const-access data without bounds checking
            // @param i: index
            // @returns: copy of element
            T operator[](size_t i) const;

            // @brief access data with bounds checking
            // @brief i: index in [0, N]
            // @returns: reference to element
            T& at(size_t i);

            // @brief const-access data with bounds checking
            // @brief i: index in [0, N]
            // @returns: copy of element
            T at(size_t i) const;

            // @brief vector-vector elementwise-arithmetics
            // @param other: vector of same size and type
            // @returns: resulting vector
            Vector<T, N> operator+(const Vector<T, N>& other) const;
            Vector<T, N> operator-(const Vector<T, N>& other) const;
            using Eigen::Array<T, 1, N>::operator*;
            using Eigen::Array<T, 1, N>::operator/;

            // @brief vector-vector elementwise assignment
            // @param other: vector of same size and type
            // @returns: reference to self after modification
            using Eigen::Array<T, 1, N>::operator+=;
            using Eigen::Array<T, 1, N>::operator-=;
            using Eigen::Array<T, 1, N>::operator*=;
            using Eigen::Array<T, 1, N>::operator/=;

            // @brief element-wise boolean operators
            // @param other: vector of same size and type
            // @returns: true if for all i: (*this)[i] == other[i], false otherwise
            bool operator==(const Vector<T, N>& other) const;
            bool operator!=(const Vector<T, N>& other) const;

            // @brief perform elementwise vector-scalar artihmetics
            // @param scalar: scalar of same type as vectors elements
            // @returns: resulting vector
            Vector<T, N> operator+(T scalar) const;
            Vector<T, N> operator-(T scalar) const;
            Vector<T, N> operator*(T scalar) const;
            Vector<T, N> operator/(T scalar) const;
            Vector<T, N> operator%(T scalar) const;

            // @brief perform elementwise vector-scalar assignment
            // @param scalar: scalar of same type as vectors elements
            // @returns: reference to self
            Vector<T, N>& operator+=(T scalar);
            Vector<T, N>& operator-=(T scalar);
            Vector<T, N>& operator*=(T scalar);
            Vector<T, N>& operator/=(T scalar);
            Vector<T, N>& operator%=(T scalar);

            // @brief assign scalar to all elements
            // @param : scalar
            // @returns reference to self
            Vector<T, N>& operator=(T);

            // @brief elementwise boolean and bitwise arithmetics
            // @param : scalar
            // @returns resulting vector
            Vector<T, N> operator&(T t) const;
            Vector<T, N> operator&&(T t) const;
            Vector<T, N> operator|(T t) const;
            Vector<T, N> operator||(T t) const;
            Vector<T, N> operator^(T t) const;

            // @brief elementwise bitwise assignment
            // @param : scalar
            // @returns reference to self
            Vector<T, N>& operator&=(T t);
            Vector<T, N>& operator|=(T t);
            Vector<T, N>& operator^=(T t);

            // @brief unary bitwise and boolean operators
            // @returns resulting vector
            Vector<T, N> operator~() const;
            Vector<T, N> operator!() const;

            // @brief compare all elements to the same scalar
            // @param : scalar
            // @returns true if for all i: (*this)[i] == scalar, false otherwise
            bool operator==(T) const;
            bool operator!=(T) const;

            // @brief expose size
            // @returns size
            static size_t size();

            using Eigen::Array<T, 1, N>::x;
            using Eigen::Array<T, 1, N>::y;
            using Eigen::Array<T, 1, N>::z;
            using Eigen::Array<T, 1, N>::w;

        private:
            virtual inline void make_polymorphic_with_this_dummy() {};
    };

    // typedefs for commonly used Ts
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

#include ".src/vector.inl"