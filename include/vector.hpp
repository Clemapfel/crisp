// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    // Vector class that behaves like a scalar in notation
    template<typename T, size_t N>
    class Vector : public Eigen::Array<T, 1, N>
    {
        static_assert(std::is_integral<T>::value or std::is_floating_point<T>::value);
        
        public:
            // @brief default CTOR
            Vector();
            
            // @brief construct from initializer list
            Vector(std::initializer_list<T> list);

            // @brief access data
            T& operator[](size_t i);
            T& at(size_t i);
            T at(size_t i) const;

            // @brief vector-vector arithmetics
            Vector<T, N> operator+(const Vector<T, N>& other) const;
            Vector<T, N> operator-(const Vector<T, N>& other) const;
            using Eigen::Array<T, 1, N>::operator*;
            using Eigen::Array<T, 1, N>::operator/;
            
            using Eigen::Array<T, 1, N>::operator+=;
            using Eigen::Array<T, 1, N>::operator-=;
            using Eigen::Array<T, 1, N>::operator*=;
            using Eigen::Array<T, 1, N>::operator/=;
            
            bool operator==(const Vector<T, N>& other) const;
            bool operator!=(const Vector<T, N>& other) const;

            // @brief perform elementwise vector-scalar artihmetics
            Vector<T, N> operator+(T scalar) const;
            Vector<T, N> operator-(T scalar) const;
            Vector<T, N> operator*(T scalar) const;
            Vector<T, N> operator/(T scalar) const;
            
            Vector<T, N>& operator+=(T scalar);
            Vector<T, N>& operator-=(T scalar);
            Vector<T, N>& operator*=(T scalar);
            Vector<T, N>& operator/=(T scalar);
            
            bool operator==(T scalar) const;
            bool operator!=(T scalar) const;
            
            // @brief expose size
            static size_t size();
            
        protected:
            using Eigen::Array<T, 1, N>::x;
            using Eigen::Array<T, 1, N>::y;
            using Eigen::Array<T, 1, N>::z;
            using Eigen::Array<T, 1, N>::w;
    };

    // class specialization for n 0 1
    template<typename T>
    struct Scalar : public Vector<T, 1>
    {
        Scalar() = default;
        Scalar(T);

        // @brief non-explicit casting operator to true arithmetic type
        operator T();
    };

    // class specialization for n = 2
    template<typename T>
    struct Vector2 : public Vector<T, 2>
    {
        Vector2() = default;
        Vector2(T x, T y);
        Vector2(Vector<T, 2>);

        using Vector<T, 2>::x;
        using Vector<T, 2>::y;
    };

    // class specialization for n = 3
    template<typename T>
    struct Vector3 : public Vector<T, 3>
    {
        Vector3() = default;
        Vector3(T x, T y, T z);
        Vector3(Vector<T, 3>);

        using Vector<T, 3>::x;
        using Vector<T, 3>::y;
        using Vector<T, 3>::z;
    };

    // class specialization for n = 4
    template<typename T>
    struct Vector4 : public Vector<T, 4>
    {
        Vector4() = default;
        Vector4(T x, T y, T z, T w);
        Vector4(Vector<T, 4>);

        using Vector<T, 4>::x;
        using Vector<T, 4>::y;
        using Vector<T, 4>::z;
        using Vector<T, 4>::w;
    };

    // typedefs for commonly used Ts
    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int>;
    using Vector2ui = Vector2<unsigned int>;

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int>;
    using Vector3ui = Vector3<unsigned int>;

    using Vector4f = Vector4<float>;
    using Vector4i = Vector4<int>;
    using Vector4ui = Vector4<unsigned int>;
}