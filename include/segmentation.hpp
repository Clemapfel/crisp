// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image_segment.hpp>
#include <image/multi_plane_image.hpp>
#include <image/binary_image.hpp>

#include <vector>

namespace crisp::Segmentation
{
    /// @brief extract all pixels of identical value results in number of segments equal to the number of pairwise different pixel values
    /// @param image
    /// @param min_segment_size: minimum size of segments, discard if size is equal or below minimum
    /// @returns vector of resulting segments
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_segments(const Image_t&, size_t min_segment_size = 2);

    /// @brief extract all 4-connected segments of identical value
    /// @param image
    /// @param min_segment_size: minimum size of segments, discard if size is equal or below minimum
    /// @returns vector of resulting segments
    template<typename Image_t>
    std::vector<ImageSegment> decompose_into_connected_segments(const Image_t&, size_t min_segment_size = 2);

    /// @brief compute threshold as specified
    /// @param image
    /// @param threshold the threshold, in [0, 1]
    /// @returns thresholded image as binary
    template<typename Inner_t>
    BinaryImage manual_threshold(const Image<Inner_t, 1>&, Inner_t threshold);

    /// @brief employ recursive heuristic to compute threshold
    /// @param image
    /// @returns thresholded image as binary
    template<typename Inner_t>
    BinaryImage basic_threshold(const Image<Inner_t>&);

    /// @brief compute threshold that maximizes between-cluster variance, also called otsu's method
    /// @param image
    /// @returns thresholded image as binary
    template<typename Inner_t>
    BinaryImage otsu_threshold(const Image<Inner_t>&);

    /// @brief compute local threshold by iterating through the image in a spiral pattern and remember only part of values visited so far
    /// @param image
    /// @param tail_length_factor: scales the number of remember elements, in [0, 1], (default: 0.05)
    /// @returns thresholded image as binary
    /// @complexity O(m * n + m * n * tail_length_factor)
    template<typename Inner_t>
    BinaryImage variable_threshold(const Image<Inner_t>&, float tail_length_factor = 0.05);

    /// @brief compute local threshold by considering the pixels neighborhood
    /// @param image
    /// @param neighborhood_size: range of pixels considered (default: 5)
    /// @returns thresholded image as binary
    /// @complexity O(m*n*neighborhood_size)
    template<typename Inner_t>
    BinaryImage neighborhood_threshold(const Image<Inner_t>&, size_t neighborhood_size = 5);

    template<typename T, size_t N>
    Texture<T, N> neighborhood_threshold(const Texture<T, N>&, size_t neighborhood_size, size_t correction);

    /// @brief cluster image but create n superpixels and k-means clustering inside their boundaries
    /// @param image: input image
    /// @param n_superpixels
    /// @param max_n_iterations: number of iterations allowed if convergence is not achieved earlier
    /// @returns clustered image of same value type as input image
    template<typename Image_t>
    Image_t superpixel_clustering(const Image_t&, size_t n_superpixels, size_t max_n_iterations = std::numeric_limits<size_t>::max());

    /// @brief cluster image by region growing
    /// @param image: input image
    /// @param seed_image: binary image with seeds
    /// @returns clustered image of same value type as input image, unclustered values will be show up as black (grayscale) or magenta (color)
    template<typename Image_t>
    Image_t region_growing_clustering(const Image_t&, BinaryImage seed_image);

    /// @brief cluster image using k-means with specified number of clusters
    /// @param image: input image
    /// @param n_clusters
    /// @param max_n_iterations: number of iterations allowed if convergence is not achieved earlier
    /// @returns clustered image of same value type as input image
    /// @note for crisp::ColorImage specifically a high-quality heuristic is applied to maximize color-distance between clusters
    template<typename Image_t>
    Image_t k_means_clustering(const Image_t&, size_t n_clusters, size_t max_n_iterations = std::numeric_limits<size_t>::max());
}

#include ".src/segmentation.inl"