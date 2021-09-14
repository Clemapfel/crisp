// 
// Copyright 2021 Clemens Cords
// Created on 12.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>

namespace crisp::EdgeDetection 
{
    BinaryImage threshold_gradient(const GrayScaleImage& original, float threshold)
    {
        auto kernel_x = SpatialFilter::simple_gradient_x();
        auto kernel_y = SpatialFilter::simple_gradient_y();

        BinaryImage out;

        bool should_measure = threshold == MEASURE_THRESHOLD;
        GrayScaleImage magnitude_img;
        float total_sum = 0;
        float max = 0;

        if (should_measure)
            magnitude_img.create(original.get_size().x(), original.get_size().y());
        else
            threshold *= 2;
        out.create(original.get_size().x(), original.get_size().y());

        for (long x = 0; x < original.get_size().x(); ++x)
        {
            for (long y = 0; y < original.get_size().y(); ++y)
            {
                float x_sum = 0;
                for (long s = 0; s < kernel_x.rows(); ++s)
                {
                    for (long t = 0; t < kernel_x.cols(); ++t)
                    {
                        float first = kernel_x(s, t);
                        float second = original(x + s, y + t);
                        x_sum = first * second;
                    }
                }

                float y_sum = 0;
                for (long s = 0; s < kernel_y.rows(); ++s)
                {
                    for (long t = 0; t < kernel_y.cols(); ++t)
                    {
                        float first = kernel_y(s, t);
                        float second = original(x + s, y + t);
                        y_sum += first * second;
                    }
                }

                if (should_measure)
                {
                    magnitude_img(x, y) = sqrt(x_sum * x_sum + y_sum * y_sum);
                    total_sum += magnitude_img(x, y);
                    max = std::max<float>(max, magnitude_img(x, y));
                }
                else
                    out(x, y) = sqrt(x_sum * x_sum + y_sum * y_sum) >= threshold;
            }
        }

        if (not should_measure)
            return out;

        threshold = 0.05 * max;

        for (long x = 0; x < original.get_size().x(); ++x)
            for (long y = 0; y < original.get_size().y(); ++y)
                out(x, y) = magnitude_img(x, y) >= threshold;

        return out;
    }

    // @brief detects edges via computing the sobel gradient, then thresholding the resulting image. Moderatly fast and moderately accurate
    // @param original: the image to be transformed
    // @param threshold: threshold for the gradient, range [0, 1] where 1 excludes all possible edges, 0 includes all
    // @returns binary image with edges
    //
    // @note no pre-processing such a smoothing or morphological post-processing is applied
    BinaryImage threshold_sobel(const GrayScaleImage& original, float threshold)
    {
        auto kernel_x = SpatialFilter::sobel_gradient_x();
        auto kernel_y = SpatialFilter::sobel_gradient_y();

        BinaryImage out;

        bool should_measure = threshold == MEASURE_THRESHOLD;
        GrayScaleImage magnitude_img;
        float total_sum = 0;
        float max = 0;

        if (should_measure)
            magnitude_img.create(original.get_size().x(), original.get_size().y());
        else
            threshold *= 4;
        out.create(original.get_size().x(), original.get_size().y());

        for (long x = 0; x < original.get_size().x(); ++x)
        {
            for (long y = 0; y < original.get_size().y(); ++y)
            {
                float x_sum = 0;
                for (long s : {-1, 1})  // skipping the zeros has same performance as seperating the kernels
                {
                    for (long t : {-1, 0, +1})
                    {
                        float first = kernel_x(1 + s, 1 + t);
                        float second = original(x + s, y + t);
                        x_sum += first * second;
                    }
                }

                float y_sum = 0;
                for (long s : {-1, 0, +1})
                {
                    for (long t : {-1, 1})
                    {
                        float first = kernel_y(1 + s, 1 + t);
                        float second = original(x + s, y + t);
                        y_sum += first * second;
                    }
                }

                if (should_measure)
                {
                    magnitude_img(x, y) = sqrt(x_sum * x_sum + y_sum * y_sum);
                    total_sum += magnitude_img(x, y);
                    max = std::max<float>(max, magnitude_img(x, y));
                }
                else
                    out(x, y) =  sqrt(x_sum * x_sum + y_sum * y_sum) >= threshold;
            }
        }

        if (not should_measure)
            return out;

        threshold = 0.05 * max;

        for (long x = 0; x < original.get_size().x(); ++x)
            for (long y = 0; y < original.get_size().y(); ++y)
                out(x, y) = magnitude_img(x, y) >= threshold;

        return out;
    }

    BinaryImage canny(const GrayScaleImage& original, float lower_threshold, float upper_threshold)
    {
        assert(lower_threshold <= upper_threshold);

        auto kernel_x = SpatialFilter::sobel_gradient_x();
        auto kernel_y = SpatialFilter::sobel_gradient_y();

        GrayScaleImage magnitude_img;
        magnitude_img.create(original.get_size().x(), original.get_size().y());
        magnitude_img.set_padding_type(ZERO);

        GrayScaleImage angle_img = original;
        angle_img.create(original.get_size().x(), original.get_size().y());
        angle_img.set_padding_type(STRETCH);

        float min = 0;
        float max = 4;  // maximum possible magnitude gradient in float [0, 1]
        float total_sum = 0;

        // compute magnitude and angle
        for (long x = 0; x < original.get_size().x(); ++x)
        {
            for (long y = 0; y < original.get_size().y(); ++y)
            {
                float x_sum = 0;
                for (long s : {-1, 1})  // skipping the zeros has same performance as seperating the kernels
                {
                    for (long t : {-1, 0, +1})
                    {
                        float first = kernel_x(1 + s, 1 + t);
                        float second = original(x + s, y + t);
                        x_sum += first * second;
                    }
                }

                float y_sum = 0;
                for (long s : {-1, 0, +1})
                {
                    for (long t : {-1, 1})
                    {
                        float first = kernel_y(1 + s, 1 + t);
                        float second = original(x + s, y + t);
                        y_sum += first * second;
                    }
                }

                magnitude_img(x, y) = sqrt(x_sum * x_sum + y_sum * y_sum);
                angle_img(x, y) = atan2(x_sum, y_sum) * (180 / M_PI);

                min = std::min<float>(magnitude_img(x, y), min);
                max = std::max<float>(magnitude_img(x, y), max);
                total_sum += magnitude_img(x, y);
            }
        }

        float mean = total_sum / (original.get_size().x()*original.get_size().y());
        float lower = (lower_threshold == -1 ? mean : lower_threshold * 4);
        float upper = (upper_threshold == -1 ? 2*mean : upper_threshold * 4);

        BinaryImage out;
        out.create(original.get_size().x(), original.get_size().y());

        // find weak and strong edges and link
        for (long x = 0; x < original.get_size().x(); ++x)
        {
            for (long y = 0; y < original.get_size().y(); ++y)
            {
                Eigen::Matrix<float, 3, 3> neighborhood;
                for (int i = -1; i <= 1; i++)
                    for (int j = -1; j <= 1; j++)
                        neighborhood(i + 1, j + 1) = magnitude_img(x + i, y + j);

                bool has_neighbor = false;
                float magnitude = magnitude_img(x, y);
                float angle = angle_img(x, y);

                // horizontal
                if ((angle < -67.5 and angle > -112.5) or (angle > 67.5 and angle < 112.5))
                {
                    if ((magnitude > neighborhood(0, 1)) and (magnitude > neighborhood(2, 1)))
                        out(x, y) = 0.1;

                    if ((neighborhood(0, 1) > lower) or (neighborhood(2, 1) > lower))
                        has_neighbor = true;
                }
                // vertical
                else if ((angle > -22.5 and angle < 22.5) or (angle < -157.5 and angle > -180) or (angle > 157.5 and angle < 180))
                {
                    if ((magnitude > neighborhood(1, 0)) and (magnitude > neighborhood(1, 2)))
                        out(x, y) = 0.1;

                    if ((neighborhood(1, 0) > lower) or (neighborhood(1, 2) > lower))
                        has_neighbor = true;
                }
                // +45°
                else if ((angle > 112.5 and angle < 157.5) or (angle < -22.5 and angle > -67.5))
                {
                    if ((magnitude > neighborhood(0, 2)) and (magnitude > neighborhood(2, 0)))
                        out(x, y) = 0.1;

                    if ((neighborhood(0, 2) > lower) or (neighborhood(2, 0) > lower))
                        has_neighbor = true;
                }
                // -45°
                else if ((angle > 22.5 and angle < 67.5) or (angle < -112.5 and angle > -157.5))
                {
                    if ((magnitude > neighborhood(0, 0)) and (magnitude > neighborhood(2, 2)))
                        out(x, y) = 0.1;

                    if ((neighborhood(0, 0) > lower) or (neighborhood(2, 2) > lower))
                        has_neighbor = true;
                }

                if (out(x, y))
                {
                    if (magnitude >= upper and has_neighbor)
                            out(x, y) = true;
                    else
                        out(x, y) = false;
                }
            }
        }

        // post process away small edges
        for (long x = 0; x < out.get_size().x(); ++x)
            for (long y = 0; y < out.get_size().y(); ++y)
            {
                if (out(x, y) != true)
                    continue;

                size_t outer_count = 0;
                size_t inner_count = 0;
                for (int i = -2; i <= 2; i++)
                    for (int j = -2; j <= 2; j++)
                    {
                        if (out(x + i, y + j) == true)
                        {
                            if (abs(i) == 1 or abs(j) == 1)
                                inner_count += 1;
                            else if (abs(i) == 2 or abs(j) == 2)
                                outer_count += 1;

                            if (inner_count + outer_count >= 2 and inner_count > 0)
                                goto skip;
                        }
                    }

                out(x, y) = false;
                skip:;
            }

        return out;
    }
}