// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    using StructuringElement = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;

    class MorphologicalTransform
    {
        public:
            void set_structuring_element(StructuringElement);

            template<typename Image_t>
            void erode(Image_t& image);

    };
}