// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>

#include <set>

namespace crisp
{
    namespace detail
    {
        struct PixelCoordCompare
        {
            bool operator()(const Vector2ui& a, const Vector2ui& b) const
            {
                return a.y() != b.y() ? a.y() < b.y() : a.x() < b.x();
            }
        };
    }

    /// @brief a set of unique pixel coordinates
    using ImageSegment = std::set<Vector2ui, detail::PixelCoordCompare>;
}