// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    // @brief Vector class that behaves like a scalar, all arithmetics are element-wise
    // @param T: value type, must be arithmetic
    // @param N: number of components
    template<typename T, size_t N>
    class Vector : public Eigen::Array<T, 1, N>
    {
        static_assert(std::is_integral<T>::value or std::is_floating_point<T>::value);

        public:
            // @brief expose T as member typedef
            using Value_t = T;

            // @brief default ctor
            Vector();

            // @brief construct variadic
            Vector(std::initializer_list<T>);

            // @brief ctor and assign all elements
            explicit (N != 1) Vector(T);

            // @brief decays vector to element for N = 1, throws static assertion if N > 1
            explicit (N != 1) operator T() const;

            // @brief access data without bounds checking
            T& operator[](size_t i);

            // @brief const-access data without bounds checking
            T operator[](size_t i) const;

            // @brief access data with bounds checking
            T& at(size_t i);

            // @brief const-access data with bounds checking
            T at(size_t i) const;

            // @brief translate to hash, hash collisions will appear if any component is outside of [0,1]
            size_t to_hash() const;

            // @brief vector-vector elementwise arithmetics
            Vector<T, N> operator+(const Vector<T, N>& other) const;
            Vector<T, N> operator-(const Vector<T, N>& other) const;
            using Eigen::Array<T, 1, N>::operator*;
            using Eigen::Array<T, 1, N>::operator/;

            // @brief vector-vector, vector-scalar elementwise assignment
            using Eigen::Array<T, 1, N>::operator+=;
            using Eigen::Array<T, 1, N>::operator-=;
            using Eigen::Array<T, 1, N>::operator*=;
            using Eigen::Array<T, 1, N>::operator/=;

            Vector<T, N>& operator%=(const Vector<T, N>& other);
            Vector<T, N>& operator%=(T scalar);

            // @brief assigns scalar to all components
            Vector<T, N>& operator=(T);

            // @brief vector-scalar elementwise arithmetics
            Vector<T, N> operator+(T scalar) const;
            Vector<T, N> operator-(T scalar) const;
            Vector<T, N> operator*(T scalar) const;
            Vector<T, N> operator/(T scalar) const;
            Vector<T, N> operator%(T scalar) const;

            // @brief elementwise boolean and bitwise arithmetics
            Vector<T, N> operator&(T t) const;
            Vector<T, N> operator&&(T t) const;
            Vector<T, N> operator|(T t) const;
            Vector<T, N> operator||(T t) const;
            Vector<T, N> operator^(T t) const;

            // @brief elementwise bitwise assignment
            Vector<T, N>& operator&=(T t);
            Vector<T, N>& operator|=(T t);
            Vector<T, N>& operator^=(T t);

            // @brief unary bitwise and boolean operators
            Vector<T, N> operator~() const;
            using Eigen::Array<T, 1, N>::operator!;

            // @brief vector-vector elementwise equality operators
            bool operator==(const Vector<T, N>& other) const;
            bool operator!=(const Vector<T, N>& other) const;

            // @brief vector-vector elementwise comparison operators
            // @note vectors are ordered based on their respective hash, so for vectors a, b: a > b iff hash(a) > hash(b)
            bool operator<(const Vector<T, N>&) const;
            bool operator<=(const Vector<T, N>&) const;
            bool operator>(const Vector<T, N>&) const;
            bool operator>=(const Vector<T, N>&) const;

            // @brief vector-scalar elemetnwise comparison operators
            bool operator==(T) const;
            bool operator!=(T) const;
            bool operator<(T) const;
            bool operator<=(T) const;
            bool operator>(T) const;
            bool operator>=(T) const;

            // @brief get number of components
            static size_t size();

            // @brief access elements
            using Eigen::Array<T, 1, N>::x;
            using Eigen::Array<T, 1, N>::y;
            using Eigen::Array<T, 1, N>::z;
            using Eigen::Array<T, 1, N>::w;

        private:
            virtual inline void make_polymorphic_with_this_dummy()
            {};
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

// hash
template<typename T, size_t N>
struct std::hash<crisp::Vector<T, N>>;

#include ".src/vector.inl"