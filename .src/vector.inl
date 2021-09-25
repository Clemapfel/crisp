// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <cstdarg>
#include <.src/common.inl>
#include <vector.hpp>


namespace crisp
{
    template<typename T, size_t N>
    Vector<T, N>::Vector() noexcept
            : Eigen::Array<T, 1, N>()
    {
        Eigen::Array<T, 1, N>::setConstant(0);
    }

    template<typename T, size_t N>
    Vector<T, N>::Vector(std::initializer_list<T> list)
    {
        assert(list.size() == N);

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

    template<typename T, size_t N>
    Vector<T, N>::operator T() const
    {
        static_assert(N == 1, "explicit type casting is only supported for vectors of length 1");
        return static_cast<T>(Eigen::Array<T, 1, N>::operator()(0, 0));
    }

    template<typename T, size_t N>
    T& Vector<T, N>::operator[](size_t i) noexcept
    {
        return Eigen::Array<T, 1, N>::operator()(0, i);
    }

    template<typename T, size_t N>
    T Vector<T, N>::operator[](size_t i) const noexcept
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
    size_t Vector<T, N>::to_hash() const
    {
        return std::hash<Vector<T, N>>{}(*this);
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator+(const Vector <T, N>& other) const noexcept
    {
        auto out = *this;
        out += other;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator-(const Vector <T, N>& other) const noexcept
    {
        auto out = *this;
        out *= other;
        return out;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator==(const Vector <T, N>& other) const noexcept
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) != other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator!=(const Vector <T, N>& other) const noexcept
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) == other.at(i))
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator<(const Vector <T, N>& other) const noexcept
    {
        return (*this).to_hash() < other.to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator<=(const Vector <T, N>& other) const noexcept
    {
        return (*this).to_hash() <= other.to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator>(const Vector <T, N>& other) const noexcept
    {
        return (*this).to_hash() > other.to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator>=(const Vector <T, N>& other) const noexcept
    {
        return (*this).to_hash() >= other.to_hash();
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator+(T scalar) const noexcept
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) += scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator-(T scalar) const noexcept
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) -= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator*(T scalar) const noexcept
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) *= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator/(T scalar) const noexcept
    {
        auto out = *this;
        for (size_t i = 0; i < N; ++i)
            out.at(i) /= scalar;
        return out;
    }

    template<typename T, size_t N>
    Vector <T, N> Vector<T, N>::operator%(T scalar) const noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) % scalar;
        return *this;
    }

    template<typename T, size_t N>
    Vector <T, N>& Vector<T, N>::operator%=(const Vector<T, N>& other) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) %= other.at(i);
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator%=(T scalar) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) %= scalar;
        return *this;
    }

    template<typename T, size_t N>
    constexpr size_t Vector<T, N>::size() noexcept
    {
        return N;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator=(T t) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) = t;

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&(T t) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) & t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&&(T t) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) && t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator|(T t) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) | t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator||(T t) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) || t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator^(T t) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = this->at(i) ^ t;

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator~() const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out = ~(this->at(i));

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator&=(T t) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) &= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator|=(T t) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) |= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator^=(T t) noexcept
    {
        for (size_t i = 0; i < N; ++i)
            this->at(i) |= t->at(i);

        return *this;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator==(T t) const noexcept
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) != t)
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator!=(T t) const noexcept
    {
        for (size_t i = 0; i < N; ++i)
            if (this->at(i) == t)
                return false;

        return true;
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator<(T t) const noexcept
    {
        if (N == 1)
            return T(*this) < t;
        else
            return (*this).to_hash() < Vector<T, N>(t).to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator<=(T t) const noexcept
    {
        if (N == 1)
            return T(*this) <= t;
        else
            return (*this).to_hash() <= Vector<T, N>(t).to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator>(T t) const noexcept
    {
        if (N == 1)
            return T(*this) > t;
        else
            return (*this).to_hash() > Vector<T, N>(t).to_hash();
    }

    template<typename T, size_t N>
    bool Vector<T, N>::operator>=(T t) const noexcept
    {
        if (N == 1)
            return T(*this) > t;
        else
            return (*this).to_hash() >= Vector<T, N>(t).to_hash();
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&(const Vector<T, N>& other) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out.at(i) = (*this).at(i) & other.at(i);

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator|(const Vector<T, N>& other) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out.at(i) = (*this).at(i) | other.at(i);

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator^(const Vector<T, N>& other) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out.at(i) = (*this).at(i) ^ other.at(i);

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator&&(const Vector<T, N>& other) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out.at(i) = (*this).at(i) && other.at(i);

        return out;
    }

    template<typename T, size_t N>
    Vector<T, N> Vector<T, N>::operator||(const Vector<T, N>& other) const noexcept
    {
        Vector<T, N> out;

        for (size_t i = 0; i < N; ++i)
            out.at(i) = (*this).at(i) || other.at(i);

        return out;
    }
}

template<typename T, size_t N>
struct std::hash<crisp::Vector<T, N>>
{
    size_t operator()(const crisp::Vector<T, N>& in) const
    {
        // map each component into [0, 256*256], then map onto 2 chars: AA AB .., then hash the resulting string
        int sigma = 256;
        std::string out;
        for (size_t i = 0; i < N; ++i)
        {
            int projected = float(crisp::clamp<T>(0, 1, in.at(i))) * float(65025);
            out.push_back(static_cast<char>(floor(float(projected) / float(sigma))));
            out.push_back(static_cast<char>(int(projected % sigma)));
        }

        return std::hash<std::string>{}(out);
    }
};