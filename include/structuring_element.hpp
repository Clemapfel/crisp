// 
// Copyright 2021 Clemens Cords
// Created on 25.10.21 by clem (mail@clemens-cords.com)
//

#include <Dense>

#pragma once

/// @brief matrix of std::optional<bool> used as flat structuring element, the optional element not having a value represents "don't care" elements
using StructuringElement = Eigen::Matrix<std::optional<bool>, Eigen::Dynamic, Eigen::Dynamic>;

/// @brief matrix of std::optional<float> used as non-flat structuring element
using NonFlatStructuringElement = Eigen::Matrix<std::optional<float>, Eigen::Dynamic, Eigen::Dynamic>;
