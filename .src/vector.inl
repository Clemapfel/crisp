// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

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
    // specializations: decay to trivial type for N = 1
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

    // partial specialization for N = 2
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
