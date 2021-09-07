// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <vector.hpp>

namespace crisp
{
    template<typename T, size_t N>
    Vector<T, N>::Vector()
        : Eigen::Array<T, 1, N>()
    {
        Eigen::Array<T, 1, N>::setConstant(0);
    }

    template<typename T, size_t N>
    Vector<T, N>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == N);

        size_t i = 0;
        for (auto it = list.begin(); i < N and it != list.end(); it += 1, ++i)
            Eigen::Array<T, 1, N>::operator()(0, i) = *it;
    }

    template<typename T, size_t N>
    T & Vector<T, N>::operator[](size_t i)
    {
        return Eigen::Array<T, 1, N>::operator()(0, i);
    }

    template<typename T, size_t N>
    T & Vector<T, N>::at(size_t i)
    {
        if (i >= N)
            throw std::out_of_range("Index " + std::to_string(i) + " out of range for vector of size " + std::to_string(N));

        return Eigen::Array<T, 1, N>::operator()(0, i);;
    }

    template<typename T, size_t N>
    T Vector<T, N>::at(size_t i) const
    {
        if (i >= N)
            throw std::out_of_range("Index " + std::to_string(i) + " out of range for vector of size " + std::to_string(N));

        return Eigen::Array<T, 1, N>::operator()(0, i);;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator+(const Vector<T, N>& other) const
    {
        auto out = *this;
        out += other;
        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator-(const Vector<T, N>& other) const
    {
        auto out = *this;
        out *= other;
        return out;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator==(const Vector<T, N>& other) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) != other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator!=(const Vector<T, N>& other) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) == other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator+(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) += scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator-(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) -= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator*(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) *= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator/(T scalar) const
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) /= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator%(T scalar) const
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) % scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator+=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) += scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator-=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) -= scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator%=(T scalar)
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

    template<typename T>
    Scalar<T>::Scalar(T t)
        : Vector<T, 1>({t})
    {}

    template<typename T>
    Scalar<T>::operator T() const
    {
        return Vector<T, 1>::at(0);
    }

    template<typename T>
    Scalar<T> & Scalar<T>::operator=(T t)
    {
        (*this)[0] = t;
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
}