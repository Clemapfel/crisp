// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <cstdarg>
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
        size_t i = 0;
        for (T t : list)
        {
            this->at(i) = t;
            i += 1;
        }
    }

    template<typename T, size_t N>
    Vector<T, N>::Vector(T t)
    {
        Eigen::Array<T, 1, N>::setConstant(t);
    }

    /*
    template<typename T, size_t N>
    template<typename Var_t>
    Vector<T, N>::Vector(Var_t in...)
    {
        std::va_list args;
        va_start(args, in);

        std::vector<Var_t> temp;
        for (size_t i = 0; i < N; ++i)
        {
            this->at(i) = va_arg(args, Var_t);
        }

        va_end(args);
    }*/

    template<typename T, size_t N>
    Vector<T, N>::operator T() const
    {
        if (N == 1)
            return Eigen::Array<T, 1, N>::operator()(0, 0);
        else
        {
            T sum = 0;
            for (size_t i = 0; i < N; ++i)
                sum += std::abs<T>(at(i));

            return sum;
        }
    }

    template<typename T, size_t N>
    T& Vector<T, N>::operator[](size_t i)
    {
        return Eigen::Array<T, 1, N>::operator()(0, i);
    }

    template<typename T, size_t N>
    T Vector<T, N>::operator[](size_t i) const
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

        return Eigen::Array<T, 1, N>::operator()(0, i);
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
    Vector <T, N>& Vector<T, N>::operator*=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) *= scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector <T, N>& Vector<T, N>::operator/=(T scalar)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) /= scalar;
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

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator=(T t)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) = t;

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&(T t) const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) & t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&&(T t) const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) && t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator|(T t) const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) && t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator||(T t) const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) && t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator^(T t) const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) && t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator~() const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = ~(this->at(i));

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator!() const
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = !(this->at(i));

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator&=(T t)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) &= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator|=(T t)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) |= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator^=(T t)
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) |= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator==(T t) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) != t)
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator!=(T t) const
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) == t)
                return false;

        return true;
    }
}