// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>
#include <cstdarg>

namespace crisp::detail
{
    template<typename T, size_t N>
    Vector<T, N>::Vector()
            : Eigen::Array<T, 1, N>()
    {
        Eigen::Array<T, 1, N>::setConstant(0);
    }

    template<typename T, size_t N>
    T& Vector<T, N>::operator[](size_t i)
    {
        return Eigen::Array<T, 1, N>::operator()(0, i);
    }

    template<typename T, size_t N>
    T& Vector<T, N>::at(size_t i)
    {
        if (i >= N)
            throw std::out_of_range(
                    "Index " + std::to_string(i) + " out of range for vector of size " + std::to_string(N));

        return Eigen::Array<T, 1, N>::operator()(0, i);;
    }

    template<typename T, size_t N>
    T Vector<T, N>::at(size_t i) const
    {
        if (i >= N)
            throw std::out_of_range(
                    "Index " + std::to_string(i) + " out of range for vector of size " + std::to_string(N));

        return Eigen::Array<T, 1, N>::operator()(0, i);;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator+(const Vector <T, N>& other) const
    {
        auto out = *this;
        out += other;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator-(const Vector <T, N>& other) const
    {
        auto out = *this;
        out *= other;
        return out;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator==(const Vector <T, N>& other) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) != other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator!=(const Vector <T, N>& other) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) == other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator+(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) += scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator-(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) -= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator*(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) *= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator/(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) /= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator%(T scalar) const
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) % scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector <T, N>& Vector<T, N>::operator+=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) += scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector <T, N>& Vector<T, N>::operator-=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) -= scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector <T, N>& Vector<T, N>::operator%=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) %= scalar;
        return *this;
    }

    template<typename T, size_t N>
    size_t Vector<T, N>::size()
    {
        return N;
    }
}

namespace crisp
{
    template<typename T, size_t N>
    Vector<T, N>::Vector()
        : detail::Vector<T, N>()
    {}

    template<typename T>
    Vector<T, 1>::Vector()
        : detail::Vector<T, 1>()
    {}

    template<typename T>
    Vector<T, 2>::Vector()
        : detail::Vector<T, 2>()
    {}

    template<typename T>
    Vector<T, 3>::Vector()
        : detail::Vector<T, 3>()
    {}

    template<typename T>
    Vector<T, 4>::Vector()
        : detail::Vector<T, 4>()
    {}

    template<typename T, size_t N>
    Vector<T, N>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == N);

        size_t i = 0;
        for (auto it = list.begin(); i < N and it != list.end(); it += 1, ++i)
            Eigen::Array<T, 1, N>::operator()(0, i) = *it;
    }

    template<typename T>
    Vector<T, 2>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == 2);

        size_t i = 0;
        for (auto it = list.begin(); i < 2 and it != list.end(); it += 1, ++i)
            Eigen::Array<T, 1, 2>::operator()(0, i) = *it;
    }

    template<typename T>
    Vector<T, 3>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == 3);

        size_t i = 0;
        for (auto it = list.begin(); i < 3 and it != list.end(); it += 1, ++i)
            Eigen::Array<T, 1, 3>::operator()(0, i) = *it;
    }

    template<typename T>
    Vector<T, 4>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == 4);

        size_t i = 0;
        for (auto it = list.begin(); i < 4 and it != list.end(); it += 1, ++i)
            Eigen::Array<T, 1, 4>::operator()(0, i) = *it;
    }

    template<typename T, size_t N>
    Vector<T, N>::Vector(T value)
    {
       Eigen::Array<T, 1, N>::setConstant(value);
    }

    template<typename T>
    Vector<T, 1>::Vector(T value)
    {
       Eigen::Array<T, 1, 1>::setConstant(value);
    }

    template<typename T>
    Vector<T, 2>::Vector(T value)
    {
       Eigen::Array<T, 1, 2>::setConstant(value);
    }

    template<typename T>
    Vector<T, 3>::Vector(T value)
    {
       Eigen::Array<T, 1, 3>::setConstant(value);
    }

    template<typename T>
    Vector<T, 4>::Vector(T value)
    {
       Eigen::Array<T, 1, 4>::setConstant(value);
    }

    template<typename T>
    Vector<T, 2>::Vector(T x, T y)
    {
        Vector<T, 2>::at(0) = x;
        Vector<T, 2>::at(1) = y;
    }

    template<typename T>
    Vector<T, 3>::Vector(T x, T y, T z)
    {
        Vector<T, 3>::at(0) = x;
        Vector<T, 3>::at(1) = y;
        Vector<T, 3>::at(2) = z;
    }

    template<typename T>
    Vector<T, 4>::Vector(T x, T y, T z, T w)
    {
        Vector<T, 3>::at(0) = x;
        Vector<T, 3>::at(1) = y;
        Vector<T, 3>::at(2) = z;
        Vector<T, 3>::at(3) = w;
    }
}

    /*
    template<typename T>
    Scalar<T>::Scalar(T t)
        : Vector<T, 1>({t})
    {}

    /*
    template<>
    class Scalar<bool> : public Vector<bool, 1>
    {
        inline operator bool() const
        {
            return static_cast<bool>((*this).Vector<bool, 1>::at(0));
        }
    };*/

    /*
    template<typename T>
    Scalar<T> & Scalar<T>::operator=(T t)
    {
        (*this)[0] = t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator&(T t) const
    {
        return (*this).at(0) & t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator&&(T t) const
    {
        return (*this).at(0) && t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator|(T t) const
    {
        return (*this).at(0) | t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator||(T t) const
    {
        return (*this).at(0) || t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator^(T t) const
    {
        return (*this).at(0) ^ t;
    }

    template<typename T>
    Scalar<T>& Scalar<T>::operator&=(T t)
    {
        return (*this)[0] &= t;
    }

    template<typename T>
    Scalar<T>& Scalar<T>::operator|=(T t)
    {
        return (*this)[0] &= t;
    }

    template<typename T>
    Scalar<T>& Scalar<T>::operator^=(T t)
    {
        return (*this)[0] &= t;
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator!() const
    {
        return ! (*this).at(0);
    }

    template<typename T>
    Scalar<T> Scalar<T>::operator~() const
    {
        return ~ (*this).at(0);
    }


    template<typename T>
    Vector2<T>::Vector2(T x, T y)
        : Vector<T, 2>({x, y})
    {}

    template<typename T>
    Vector2<T>::Vector2(Vector<T, 2> other)
        : Vector2<T>(other.at(0), other.at(1))
    {}

    template<typename T>
    Vector3<T>::Vector3(T x, T y, T z)
        : Vector<T, 3>({x, y, z})
    {}

    template<typename T>
    Vector3<T>::Vector3(Vector<T, 3> other)
        : Vector3<T>(other.at(0), other.at(1), other.at(2))
    {}

    template<typename T>
    Vector4<T>::Vector4(T x, T y, T z, T w)
        : Vector<T, 4>({x, y, z})
    {}

    template<typename T>
    Vector4<T>::Vector4(Vector<T, 4> other)
        : Vector4<T>(other.at(0), other.at(1), other.at(2), other.at(3))
    {}
     */
