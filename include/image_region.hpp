// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image/multi_plane_image.hpp>

#include <set>

namespace crisp 
{
    /// @brief direction of co-occurrence when traveling from index (x,y) to index (x+a,y+b) with a, b in {-1, 0, 1}
    enum CoOccurrenceDirection
    {
        /// (x,y) -> (x,   y-1)
        PLUS_MINUS_ZERO,
        /// (x,y) -> (x+1, y-1)
        PLUS_45,
        /// (x,y) -> (x+1, y)
        PLUS_90,
        /// (x,y) -> (x+1, y+1)
        PLUS_125,
        /// (x,y) -> (x,   y+1)
        PLUS_MINUS_180,
        /// (x,y) -> (x-1, y+1)
        MINUS_125,
        /// (x,y) -> (x-1, y)
        MINUS_90,
        /// (x,y) -> (x-1, y-1)
        MINUS_45
    };

    /// @brief a region is a an image segment along with the corresponding image values
    template<typename Image_t>
    class ImageRegion
    {   
        using Value_t = typename Image_t::Value_t;
        class Iterator;
        class ConstIterator;
            
        public:
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
            
            /// @brief construct region from entire image
            /// @param image:
            ImageRegion(const Image_t& image);
            
            /// @brief construct region from entire image
            /// @param image:
            void create_from(const Image_t& image);
            
            /// @brief get the centroid, the mean of the sum of boundary elements
            /// @returns centroid as vector
            Vector2f get_centroid() const;

            /// @brief get all boundary pixel positions
            /// @returns const reference to stored vector of positions
            const std::vector<Vector2ui>& get_boundary() const;

            /// @brief get all boundary polygon vertex positions
            /// @returns const reference to stored vector of positions
            const std::vector<Vector2ui>& get_boundary_polygon() const;

            /// @brief compute farthest point signature as proposed by El-ghazal, Basir, Belkasim (2009) in counter-clockwise order
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

            /// @brief get the major axis, the longer axis along the eigenvalues of the ellipses fitting the region
            /// @return pair of points that model the line
            const std::pair<Vector2f, Vector2f>& get_major_axis() const;

            /// @brief get the minor axis, the shorter axis along the eigenvalues of the ellipses fitting the region
            /// @return pair of points that model the line
            const std::pair<Vector2f, Vector2f>& get_minor_axis() const;

            /// @brief get the axis aligned bounding box where each side is parallel to the x/y axis
            /// @returns array of four points that model the box
            std::array<Vector2ui, 4> get_axis_aligned_bounding_box() const;
            
            /// @brief get length of the regions boundary
            /// @returns length
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
            const std::vector<std::vector<Vector2ui>>& get_hole_boundaries() const;
            
            /// @brief get the nths Hu moment invariant
            /// @param n: n in {1, 2, ..., 6, 7}
            /// @returns value of invariant
            float get_nths_moment_invariant(size_t n);
            
            /// @brief get maximum probability of intensity value
            /// @returns float in [0, 1]
            float get_maximum_intensity_probability() const;

            /// @brief get pearsons nths normalized moment around the mean of texture
            /// @returns value
            float get_nths_moment(size_t n) const;

            /// @brief get mean of texture
            /// @returns value
            float get_mean() const;

            /// @brief get standard deviation of texture
            /// @returns value
            float get_variance() const;

            /// @brief get 3rd standardized statistical moment of texture
            /// @returns value
            float get_skewness() const;

            /// @brief get 4th standardized statistical moment of texture
            /// @returns value
            float get_kurtosis() const;

            /// @brief get entropy of texture
            /// @returns value in [0, 1]
            float get_average_entropy() const;
            
            /// @brief get texture histogram
            /// @returns histogram of intensity values quantized into [0, 256]
            const auto& get_intensity_histogram() const;
            
            /// @brief get co-occurrence matrix (the number of occurrences of a pair of intensities) in specified direction. For images with multiple planes, intensities are the average intensity per element
            /// @param direction
            /// @returns 256x256 matrix, all elements normalized to [0, 1]
            const auto& get_co_occurrence_matrix(CoOccurrenceDirection direction) const;

            /// @brief get measure of correlation of the intensity values
            /// @returns float in [-1, 1]
            float get_intensity_correlation(CoOccurrenceDirection) const;

            /// @brief measure homogeneity, how close the elements in the co-occurrence matrices are distributed towards the diagonal
            /// @returns float in [0, 1]
            float get_homogeneity(CoOccurrenceDirection) const;

            /// @brief get co-occurrence matrices entropy
            /// @returns float in [0, ]1
            float get_entropy(CoOccurrenceDirection) const;

            /// @brief get measure of difference between co-occurring pixels
            /// @returns float in [0, 1]
            float get_contrast(CoOccurrenceDirection) const;

            /// @brief get const iterator to first element
            /// @returns const iterator
            /// @note no non-const iterator is supplied
            auto begin() const;

            /// @brief get const iterator to past-the-end element
            /// @returns const iterator
            /// @note no non-const iterator is supplied
            auto end() const;
            
        private:
            void create();

            struct Element
            {
                Element(Vector2ui, Value_t);

                Vector2ui _position;
                Value_t _value;
                float _intensity;
            };
            
            struct ElementCompare
            {
                // sorts left-to-right, top to bottom
                bool operator()(const Element& a, const Element& b) const
                {
                    return a.position.x() != b.position.x() ? a.position.x() < b.position.x() : a.position.y() < b.position.y();
                }
            };

            static constexpr inline size_t QUANTIZATION_N = 256;
            
            std::map<size_t, Element> _elements;
            
            std::vector<Vector2ui> _boundary;
            std::vector<Vector2ui> _boundary_polygon;
            std::vector<std::vector<Vector2ui>> _hole_boundaries;

            size_t _min_x, _max_x, _mean_x;
            size_t _min_y, _max_y, _mean_y;

            Vector2f _centroid;
            std::pair<Vector2f, Vector2f> _major_axis,
                                          _minor_axis;

            float _eccentricity = -1;

            Vector2ui _original_image_size;

            mutable bool _histogram_initialized = false;
            mutable Histogram<QUANTIZATION_N> _histogram;

            mutable float _intensity_mean = -1;
            mutable float _intensity_variance = -1;

            mutable float _average_entropy = -1;
            mutable float _max_probability = -1;

            mutable bool _intensity_occurrences_initialized = false;
            mutable std::map<float, size_t> _intensity_occurrences;

            mutable std::map<size_t, float> _nths_statistical_moment;
            mutable std::map<CoOccurrenceDirection, Eigen::MatrixXf> _co_occurrence_matrix;
    };
}

#include ".src/image_region.inl"