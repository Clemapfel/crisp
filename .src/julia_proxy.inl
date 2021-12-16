// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#include <julia.h>
#include <julia/julia_proxy.hpp>
#include <julia/julia_native_type.hpp>
#include <julia/julia_state.hpp>

namespace crisp::julia
{
    Proxy::Proxy(jl_value_t* v)
    {
        _value = v;
        _type = get_type(v);

        State::allocate(_value);
    }

    Proxy::~Proxy()
    {
        State::schedule_free(_value);
    }

    template<typename T>
    T Proxy::cast_to() const
    {
        static_assert(std::is_fundamental_v<T> or std::is_same_v<T, nullptr_t>);

        if (_type == NativeType::BOOL)
        {
            auto temp = jl_unbox_bool(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::INT_8)
        {
            auto temp = jl_unbox_int8(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::INT_16)
        {
            auto temp = jl_unbox_int16(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::INT_32)
        {
            auto temp = jl_unbox_int32(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::INT_64)
        {
            auto temp = jl_unbox_int64(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::UINT_8)
        {
            auto temp = jl_unbox_uint8(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::UINT_16)
        {
            auto temp = jl_unbox_uint16(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::UINT_32)
        {
            auto temp = jl_unbox_uint32(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::UINT_64)
        {
            auto temp = jl_unbox_int64(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::FLOAT_16)
        {
            assert(false && "[ERROR] Casting from Float16 to C-Types is not supported.");
            auto temp = jl_unbox_uint16(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::FLOAT_32)
        {
            auto temp = jl_unbox_float32(_value);
            return static_cast<T>(temp);
        }
        else if (_type == NativeType::FLOAT_64)
        {
            auto temp = jl_unbox_float64(_value);
            return static_cast<T>(temp);
        }
        else //if (_type == NativeType::VOID or _type == NativeType::VOID_PTR or _type == NativeType::NOTHING)
        {
            return static_cast<T>(NULL);
        }
    }

    Proxy::operator bool()
    {
        return cast_to<bool>();
    }

    Proxy::operator char()
    {
        return cast_to<char>();
    }

    Proxy::operator float()
    {
        return cast_to<float>();
    }

    Proxy::operator double()
    {
        return cast_to<double>();
    }

    Proxy::operator uint8_t()
    {
        return cast_to<uint8_t>();
    }

    Proxy::operator uint16_t()
    {
        return cast_to<uint16_t>();
    }

    Proxy::operator uint32_t()
    {
        return cast_to<uint32_t>();
    }

    Proxy::operator uint64_t()
    {
        return cast_to<uint64_t>();
    }

    Proxy::operator int8_t()
    {
        return cast_to<int8_t>();
    }

    Proxy::operator int16_t()
    {
        return cast_to<int16_t>();
    }

    Proxy::operator int32_t()
    {
        return cast_to<int32_t>();
    }

    Proxy::operator int64_t()
    {
        return cast_to<int64_t>();
    }

    Proxy::operator nullptr_t()
    {
        return nullptr;
    }
}