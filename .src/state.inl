// 
// Copyright 2021 Clemens Cords
// Created on 16.10.21 by clem (mail@clemens-cords.com)
//

#include <texture/state.hpp>

namespace crisp
{
    ProxyID State::register_int(int v)
    {
        auto id = get_next_id();
        _ints.insert({id, v});
        return id;
    }

    ProxyID State::register_float(float v)
    {
        auto id = get_next_id();
        _ints.insert({id, v});
        return id;
    }

    ProxyID State::register_bool(bool v)
    {
        auto id = get_next_id();
        _ints.insert({id, v});
        return id;
    }

    template<typename T>
    ProxyID State::register_vec2(const Vector<T, 2>& vec)
    {
        auto id = get_next_id();
        _vec2s.insert({
            id,
            std::array<float, 2>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y())
            }
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec3(const Vector<T, 3>& vec)
    {
        auto id = get_next_id();
        _vec3s.insert({
            id,
            std::array<float, 3>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.z())
            }
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_vec4(const Vector<T, 4>& vec)
    {
        auto id = get_next_id();
        _vec4s.insert({
            id,
            std::array<float, 4>{
                static_cast<float>(vec.x()),
                static_cast<float>(vec.y()),
                static_cast<float>(vec.z()),
                static_cast<float>(vec.w())
            }
        });

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<T>& matrix)
    {
        auto id = get_next_id();
        _array_vec1s.insert({
           id,
           std::vector<float>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec1s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back(matrix(x, y));
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 2>>& matrix)
    {
        auto id = get_next_id();
        _array_vec2s.insert({
           id,
           std::vector<std::array<float, 2>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec2s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                });
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 3>>& matrix)
    {
        auto id = get_next_id();
        _array_vec3s.insert({
           id,
           std::vector<std::array<float, 3>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec3s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                    static_cast<float>(matrix(x, y).at(2)),
                });
            }
        }

        return id;
    }

    template<typename T>
    ProxyID State::register_matrix(const Matrix<Vector<T, 4>>& matrix)
    {
        auto id = get_next_id();
        _array_vec4s.insert({
           id,
           std::vector<std::array<float, 4>>(size_t(matrix.rows() * matrix.cols()))
        });

        auto& vec = _array_vec4s.at(id);

        for (size_t y = 0; y < matrix.cols(); ++y)
        {
            for (size_t x = 0; x < matrix.rows(); ++x)
            {
                vec.push_back({
                    static_cast<float>(matrix(x, y).at(0)),
                    static_cast<float>(matrix(x, y).at(1)),
                    static_cast<float>(matrix(x, y).at(2)),
                    static_cast<float>(matrix(x, y).at(3)),
                });
            }
        }

        return id;
    }








}