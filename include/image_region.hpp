// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

#include <set>

namespace crisp
{
    /// @brief covariance matrix of float
    using CovarianceMatrix = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>;

    /// @brief a closed, simply connected region (or in less mathematical terms: a set of unique pixel coordinates and their corresponding values in an image)
    /// @param Image_t: type of image used for the values
    template<typename Image_t>
    class ImageRegion
    {
        public:
            /// @brief expose images value_t as member
            using Value_t = typename Image_t::Value_t;

            /// @brief non-const iterator, iterates left-to-right, top-to-bottom
            class Iterator;

            /// @brief const iterator, iterates left-to-right, top-to-bottom
            class ConstIterator;

            /// @brief default ctor
            ImageRegion() = default;

            /// @brief construct from segment and image
            /// @param segment: set of pixel coordinates
            /// @param image
            ImageRegion(const ImageSegment& segment, const Image_t& image);

            /// @brief construct from segment and image
            /// @param segment: set of pixel coordinates
            /// @param image
            void create_from(const ImageSegment& segment, const Image_t& image);

            /// @brief get the centroid, the mean of the sum of boundary elements
            /// @returns centroid as vector
            Vector2f get_centroid() const;

            /// @brief non-const begin
            /// @returns iterator pointing to first element
            auto begin();

            /// @brief const begin
            /// @returns const iterator pointing to first element
            auto begin() const;

            /// @brief const end
            /// @returns iterator pointing to past-the-end element
            auto end();

            /// @brief const end
            /// @returns const iterator pointing to past-the-end element
            auto end() const;

            /// @brief get all boundary pixel positions
            /// @returns const reference to stored vector of positions
            const std::vector<Vector2ui>& get_boundary() const;

            /// @brief get all boundary polygon vertex positions
            /// @returns const reference to stored vector of positions
            const std::vector<Vector2ui>& get_boundary_polygon() const;

            /// @brief compute farthers point signature as proposed by El-ghazal, Basir, Belkasim (2009) in counter-clockwise order
            /// @returns vector of distances
            /// @notes El-ghazal, A., Basir, O., & Belkasim, S. (2009). Farthest point distance: A new shape signature for Fourier descriptors. Signal Processing: Image Communication, 24(7), 572–586. doi:10.1016/j.image.2009.04.00
            std::vector<float> farthest_point_signature() const;

            /// @brief compute radial distance signature, the distance from each of the boundary point to it's centroid in counter-clockwise order
            /// @returns vector of distances
            std::vector<float> radial_distance_signature() const;

            /// @brief compute complex coordinate signature, the set of complex numbers so that the real part is the x-coordinate, the imaginary part the y-coordinate of the boundary
            /// @returns vector of complex numbers
            std::vector<std::complex<float>> complex_coordinate_signature() const;

            /// @brief compute the slope chain code signature, the angles between the vertices of the boundary, in counter-clockwise order
            /// @returns vector of angles in radian
            std::vector<float> slope_chain_code_signature() const;

            /// @brief get covariance matrix of position
            /// @returns covariance matrix of the position of each element
            const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& get_covariance_matrix() const;

            /// @brief get the major axis, the longer axis along the eigenvalues of the ellipses fitting the region
            /// @return pair of points that model the line
            const std::pair<Vector2f, Vector2f>& get_major_axis() const;

            /// @brief get the minor axis, the shorter axis along the eigenvalues of the ellipses fitting the region
            /// @return pair of points that model the line
            const std::pair<Vector2f, Vector2f>& get_minor_axis() const;

            /// @brief get bounding box where each side is parallel to the corresponding minor/major axis
            /// @returns array of four points that model the box
            std::array<Vector2ui, 4> get_bounding_box() const;

            /// @brief get the axis aligned bounding box where each side is parallel to the x/y axis
            /// @returns array of four points that model the box
            std::array<Vector2ui, 4> get_axis_aligned_bounding_box() const;

            /// @brief get length of the regions boundary
            /// @returns legnth
            float get_perimeter() const;

            /// @brief get number of pixels in region including it's boundary
            /// @returns area
            float get_area() const;

            /// @brief get compactness: (perimeter*perimeter) / area
            /// @returns compactness
            float get_compactness() const;

            /// @brief get circularity: degree of roundness of the region
            /// @returns circularity as float
            float get_circularity() const;

            /// @brief get eccentricity: degree of fit to an ellipses
            /// @returns eccentricity as float
            float get_eccentricity() const;

            /// @brief get number of holes
            /// @returns number of holes
            size_t get_n_holes() const;

            /// @brief get boundaries for holes
            /// @returns vector of vectors where each of them is the closed boundary of a hole, enumerated in counter-clockwise direction
            const std::vector<std::vector<Vector2ui>> get_hole_boundaries() const;

            /// @brief get the nths moment invariant
            /// @param n: n in {1, 2, ..., 6, 7}
            /// @returns value of invariant
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

            void compute_boundary();

            std::vector<Vector2ui> _boundary;
            std::vector<uint8_t> _boundary_direction;
            std::vector<Vector2ui> _boundary_polygon;

            std::vector<std::vector<Vector2ui>> _hole_boundaries;

            Vector2f _centroid;

            std::pair<Vector2f, Vector2f> _major_axis,
                                          _minor_axis;
            double _eccentricity;

            Vector2ui _x_bounds,
                      _y_bounds;

            size_t _n_holes;
    };

    /// @brief split a segment into multiple closed, 4-connected regions
    template<typename Image_t>
    std::vector<ImageRegion<Image_t>> decompose_into_regions(const ImageSegment&, const Image_t&);

}

#include ".src/image_region.inl"