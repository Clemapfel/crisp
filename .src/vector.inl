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

template<>
struct std::hash<crisp::Vector<size_t, 1>>
{
    size_t operator()(const crisp::Vector<size_t, 1>& in) const
    {
        return in.at(0);
    }
};

template<>
struct std::hash<crisp::Vector<int, 1>>
{
    size_t operator()(const crisp::Vector<int, 1>& in) const
    {
        return static_cast<size_t>(static_cast<uint32_t>(in.at(0) + std::numeric_limits<int32_t>::min()));
    }
};

template<>
struct std::hash<crisp::Vector<float, 1>>
{
    size_t operator()(const crisp::Vector<float, 1>& in) const
    {
        assert(in.at(0) >= 0 and in.at(0) <= 1 && "Hashing for float vectors is only supported for value range [0, 1], you need to normalize the vector into that interval or use another vector type");

        return static_cast<size_t>(double(in.at(0)) * std::numeric_limits<double>::max());
    }
};

template<>
struct std::hash<crisp::Vector<double, 1>>
{
    size_t operator()(const crisp::Vector<double, 1>& in) const
    {
        assert(in.at(0) >= 0 and in.at(0) <= 1 && "Hashing for float vectors is only supported for value range [0, 1], you need to normalize the vector into that interval or use another vector type");

        return static_cast<size_t>(in.at(0) * std::numeric_limits<double>::max());
    }
};

template<>
struct std::hash<crisp::Vector2ui>
{
    size_t operator()(const crisp::Vector2ui& in) const
    {
        size_t sigma = std::numeric_limits<uint32_t>::max() - 1;

        for (size_t i = 0; i < in.size(); ++i)
            assert(in.at(i) < sigma);

        uint32_t a = in.at(0);
        uint32_t b = in.at(1);

        uint64_t out = a;
        out << 32;
        out |= b;

        return out;
    }
};

template<>
struct std::hash<crisp::Vector2i>
{
    size_t operator()(const crisp::Vector2ui& in) const
    {
        auto a = static_cast<uint32_t>(in.at(0) + std::numeric_limits<int32_t>::max());
        auto b = static_cast<uint32_t>(in.at(1) + std::numeric_limits<int32_t>::max());

        int64_t out = a;
        out << 32;
        out |= b;

        return out;
    }
};

template<>
struct std::hash<crisp::Vector2f>
{
    size_t operator()(const crisp::Vector2ui& in) const
    {
        assert(in.at(0) >= 0 and in.at(0) <= 1 and in.at(1) >= 0 and in.at(2) <= 1);

        uint32_t sigma = std::numeric_limits<uint32_t>::max() - 1;

        auto a = static_cast<uint32_t>(in.at(0) * sigma);
        auto b = static_cast<uint32_t>(in.at(1) * sigma);

        int64_t out = a;
        out << 32;
        out |= b;

        return out;
    }
};

template<>
struct std::hash<crisp::Vector<double, 2>>
{
    size_t operator()(const crisp::Vector<double, 2>& in) const
    {
        assert(in.at(0) >= 0 and in.at(0) <= 1 and in.at(1) >= 0 and in.at(2) <= 1);

        uint32_t sigma = std::numeric_limits<uint32_t>::max() - 1;

        auto a = static_cast<uint32_t>(static_cast<float>(in.at(0)) * sigma);
        auto b = static_cast<uint32_t>(static_cast<float>(in.at(1)) * sigma);

        int64_t out = a;
        out << 32;
        out |= b;

        return out;
    }
};

template<size_t N>
struct std::hash<crisp::Vector<size_t, N>>
{
    static_assert(N == 3 or N == 4);

    size_t operator()(const crisp::Vector<size_t, N>& in) const
    {
        size_t sigma = std::numeric_limits<uint16_t>::max() - 1;

        for (size_t i = 0; i < in.size(); ++i)
            assert(in.at(i) < sigma);

        std::vector<uint16_t> components;

        for (size_t i = 0; i < in.size(); ++i)
            components.push_back(static_cast<uint16_t>(in.at(i)));

        uint64_t out = components.front();

        for (size_t i = 1; i < in.size(); ++i)
        {
            out << 16;
            out |= components.at(i);
        }

        return out;
    }
};

template<size_t N>
struct std::hash<crisp::Vector<int, N>>
{
    static_assert(N == 3 or N == 4);

    size_t operator()(const crisp::Vector<int, N>& in) const
    {
        size_t sigma = std::numeric_limits<uint16_t>::max() - 1;

        for (size_t i = 0; i < in.size(); ++i)
            assert(in.at(i) < sigma);

        std::vector<uint16_t> components;

        for (size_t i = 0; i < in.size(); ++i)
            components.push_back(static_cast<uint16_t>(in.at(i) + std::numeric_limits<int>::max()));

        uint64_t out = components.front();

        for (size_t i = 1; i < in.size(); ++i)
        {
            out << 16;
            out |= components.at(i);
        }

        return out;
    }
};

template<size_t N>
struct std::hash<crisp::Vector<float, N>>
{
    static_assert(N == 3 or N == 4);

    size_t operator()(const crisp::Vector<float, N>& in) const
    {
        for (size_t i = 0; i < in.size(); ++i)
            assert(in.at(i) >= 0 and in.at(i) <= 1);

        uint16_t sigma = std::numeric_limits<uint16_t>::max() - 1;

        std::vector<uint16_t> components;

        for (size_t i = 0; i < in.size(); ++i)
            components.push_back(static_cast<uint16_t>(in.at(i) * std::numeric_limits<uint16_t>::max()));

        uint64_t out = components.front();

        for (size_t i = 1; i < in.size(); ++i)
        {
            out << 16;
            out |= components.at(i);
        }

        return out;
    }
};

template<size_t N>
struct std::hash<crisp::Vector<double, N>>
{
    static_assert(N == 3 or N == 4);

    size_t operator()(const crisp::Vector<double, N>& in) const
    {
        for (size_t i = 0; i < in.size(); ++i)
            assert(in.at(i) >= 0 and in.at(i) <= 1);

        uint16_t sigma = std::numeric_limits<uint16_t>::max() - 1;

        std::vector<uint16_t> components;

        for (size_t i = 0; i < in.size(); ++i)
            components.push_back(static_cast<uint16_t>(static_cast<float>(in.at(i)) * std::numeric_limits<uint16_t>::max()));

        uint64_t out = components.front();

        for (size_t i = 1; i < in.size(); ++i)
        {
            out << 16;
            out |= components.at(i);
        }

        return out;
    }
};