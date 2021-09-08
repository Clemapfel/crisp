// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp::detail
{
    // Vector class that behaves like a scalar in notation
    template<typename T, size_t N>
    class Vector : public Eigen::Array<T, 1, N>
    {
        static_assert(std::is_integral<T>::value or std::is_floating_point<T>::value);

        public:
            Vector();

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
            Vector<T, N> operator%(T scalar) const;

            Vector<T, N>& operator+=(T scalar);
            Vector<T, N>& operator-=(T scalar);
            Vector<T, N>& operator*=(T scalar);
            Vector<T, N>& operator/=(T scalar);
            Vector<T, N>& operator%=(T scalar);

            Vector<T, N>& operator=(T);

            Vector<T, N> operator&(T t) const;
            Vector<T, N> operator&&(T t) const;
            Vector<T, N> operator|(T t) const;
            Vector<T, N> operator||(T t) const;
            Vector<T, N> operator^(T t) const;

            Vector<T, N>& operator&=(T t);
            Vector<T, N>& operator|=(T t);
            Vector<T, N>& operator^=(T t);

            Vector<T, N> operator~() const;
            Vector<T, N> operator!() const;

            bool operator==(T scalar) const;
            bool operator!=(T scalar) const;

            // @brief expose size
            static size_t size();

        protected:
            using Eigen::Array<T, 1, N>::x;
            using Eigen::Array<T, 1, N>::y;
            using Eigen::Array<T, 1, N>::z;
            using Eigen::Array<T, 1, N>::w;

        private:
            virtual inline void make_polymorphic_with_dummy() {};
    };
}

namespace crisp
{

    // primary template
    template<typename T, size_t N>
    class Vector : public detail::Vector<T, N>
    {
        public:
            Vector();
            Vector(std::initializer_list<T>);
            Vector(T);
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

    // specializations
    template<typename T>
    struct Vector<T, 1> : public detail::Vector<T, 1>
    {
        Vector();
        Vector(T t);

        inline operator T() const
        {
            return static_cast<T>(Eigen::Array<T, 1, 1>::operator()(0, 0));
        }

        inline Vector<T, 1>& operator=(T t)
        {
            Eigen::Array<T, 1, 1>::operator()(0, 0) = t;
        }
    };

    template<typename T>
    class Vector<T, 2> : public detail::Vector<T, 2>
    {
        public:
            Vector();
            Vector(std::initializer_list<T>);
            Vector(T);
            Vector(T x, T y);

            using detail::Vector<T, 2>::x;
            using detail::Vector<T, 2>::y;
    };


    template<typename T>
    class Vector<T, 3> : public detail::Vector<T, 3>
    {
        public:
            Vector();
            Vector(std::initializer_list<T>);
            Vector(T);
            Vector(T x, T y, T z);

            using detail::Vector<T, 3>::x;
            using detail::Vector<T, 3>::y;
            using detail::Vector<T, 3>::z;
    };

    template<typename T>
    class Vector<T, 4> : public detail::Vector<T, 4>
    {
        public:
            Vector();
            Vector(std::initializer_list<T>);
            Vector(T);
            Vector(T x, T y, T z, T w);

            using detail::Vector<T, 4>::x;
            using detail::Vector<T, 4>::y;
            using detail::Vector<T, 4>::z;
            using detail::Vector<T, 4>::w;
    };
}

#include ".src/vector.inl"