// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <Dense>

namespace crisp
{
    using StructuringElement = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

    class MorphologicalTransform
    {
        public:
            void set_structuring_element(StructuringElement);
            StructuringElement& get_structuring_element();
            
            float& operator()(size_t x, size_t y);
            float operator()(size_t x, size_t y) const;

            void set_structuring_element_origin(x, y);
            Vector2ui get_structuring_element_origin() const;

            template<typename Image_t>
            void erode(Image_t& image);
            
            template<typename Image_t>
            void erode(Image_t& image, const Image_t& mask);
            
            template<typename Image_t>
            void dilate(Image_t& image);
            
            template<typename Image_t>
            void dilate(Image_t& image, const Image_t& mask);
            
            template<typename Image_t>
            void open(Image_t& image);
            
            template<typename Image_t>
            void close(Image_t& image);
            
            template<typename Image_t>
            void hit_or_miss_transform(Image_t& image);

            template<typename Image_t>
            void miss_or_hit_transform(Image_t& image);
            
            static StructuringElement all_dont_care(long nrows, long ncols);
            static StructuringElement all_foreground(long nrows, long ncols);
            static StructuringElement all_background(long nrows, long ncols);
            static StructuringElement cross(long dimensions);
            static StructuringElement square(long dimensions);
            static StructuringElement diamond(long dimensions);
            static StructuringElement circle(long dimensions);
            static StructuringElement square_pyramid(long dimensions, float min_intensity = 0, float max_intensity = 1);
            static StructuringElement diamond_pyramid(long dimensions, float min_intensity = 0, float max_intensity = 1);
            static StructuringElement cone(long dimensions, float min_intensity = 0, float max_intensity = 1);
            static StructuringElement hemisphere(long dimensions, float min_intensity = 0, float max_intensity = 1);

        private:
            Vector2ui _origin;
            StructuringElement _structuring_element;
    };
}