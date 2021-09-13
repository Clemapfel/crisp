// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

#include <set>

namespace crisp
{
    using CovarianceMatrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

    template<typename Image_t>
    class ImageRegion
    {
        public:
            using Value_t = typename Image_t::value_t;
            class Iterator;
            class ConstIterator;

            ImageRegion() = default;
            ImageRegion(const ImageSegment& segment, const Image_t& image);
            void create_from(const ImageSegment& segment, const Image_t& image);

            Vector2f get_centroid() const;

            auto begin();
            auto begin() const;
            auto end();
            auto end() const;

            std::vector<float> farthest_point_signature() const;
            std::vector<float> radial_distance_signature() const;
            std::vector<std::complex<float>> complex_coordinate_signature() const;
            std::vector<float> slope_chain_code_signature() const;

            const std::pair<Vector2ui, Vector2ui>& get_major_axis();
            const std::pair<Vector2ui, Vector2ui>& get_minor_axis();
            std::array<Vector2ui, 4> get_bounding_box();
            std::array<Vector2ui, 4> get_axis_aligned_bounding_box();

            float get_perimeter() const;
            float get_area() const;
            float get_compactness() const;
            float get_circularity() const;
            float get_eccentricity() const;

            size_t get_n_holes() const;

            Image<float, 1> get_covariance_matrix() const;
            Image<float, 1> get_co_occurence_matrix(size_t size = 256) const;
            float get_nths_moment_invariant(size_t n);

        private:
            struct Element
            {
                Vector2ui position;
                Value_t value;
            };

            struct ElementCompare
            {
                bool operator()(const Element& a, const Element& b) const
                {
                    return a.position.x() != b.position.x() ? a.position.x() < b.position.x() : a.position.y() < b.position.y();
                }
            };

            std::set<Element, ElementCompare> _elements;

            Vector2f _centroid;

            Vector2ui _x_bounds,
                      _y_bounds;

            size_t _n_holes;




    };
}