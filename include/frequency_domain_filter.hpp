// 
// Copyright 2021 Clemens Cords
// Created on 09.08.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <grayscale_image.hpp>
#include <fourier_transform.hpp>

namespace crisp
{
    // filter intended to be applied to frequency-domain transforms such as the fourier transform
    class FrequencyDomainFilter
    {
        public:
            // @brief construct the filter of a specified size
            // @param width: the x-dimension of the spectrum the filter will be applied to
            // @param height: the y-dimensions of the spectrum the filter will be applied to
            FrequencyDomainFilter(long width, long height);

            // @brief multiply the filter with a fourier specturm
            // @param : the spectrum to be modified
            template<FourierTransformMode Mode>
            void apply_to(FourierTransform<Mode>&) const;

            // @brief resize the filter
            // @param : vector where .x is the x-dimensions and .y the y-dimensions of the spectrum the filter will be applied to
            void set_size(Vector2ui);

            // @brief get the filters dimensions
            // @returns vector where .x is the width, .y the height of the filter
            Vector2ui get_size() const;

            // @brief set the function that determines the shape of the filter
            // @param lambda: function that takes the x- and y-coordinate and returns an appropraite value
            // @note identity by default;
            void set_function(std::function<double(long, long)>&& lambda);

            // @brief move the center of the function relative to the filters center origin
            // @param x_dist_from_center: x-distance, values in [-width/2, +width/2]
            // @param y_dist_from_center: y-distance, values in [-height/2, +height/2];
            // @param force_symmetry: should the filter be mirror across it's origin
            // @note not mirroring it will cause the spectrums phase angle to distort after filtering
            void set_offset(long x_dist_from_center, long y_dist_from_center, bool force_symmetry = true);

            // @brief element-wise add one filter to another
            // @param : the other filter
            // @returns resulting filter
            FrequencyDomainFilter operator+(const FrequencyDomainFilter&) const;

            // @brief element-wise subtract one filter from another
            // @param : the other filter
            // @returns resulting filter
            FrequencyDomainFilter operator-(const FrequencyDomainFilter&) const;

            // @brief element-wise add multiply one filter with another
            // @param : the other filter
            // @returns resulting filter
            FrequencyDomainFilter operator*(const FrequencyDomainFilter&) const;

            // @brief element-wise divide one filter by another
            // @param : the other filter
            // @returns resulting filter
            FrequencyDomainFilter operator/(const FrequencyDomainFilter&) const;

            // @brief assign the element-wise addition with another filter
            // @param : other filter
            // @returns reference to self after assignment
            FrequencyDomainFilter& operator+=(const FrequencyDomainFilter&);

            // @brief assign the element-wise subtraction from another filter
            // @param : other filter
            // @returns reference to self after assignment
            FrequencyDomainFilter& operator-=(const FrequencyDomainFilter&);

            // @brief assign the element-wise multiplication with another filter
            // @param : other filter
            // @returns reference to self after assignment
            FrequencyDomainFilter& operator*=(const FrequencyDomainFilter&);

            // @brief assign the element-wise division by another filter
            // @param : other filter
            // @returns reference to self after assignment
            FrequencyDomainFilter& operator/=(const FrequencyDomainFilter&);

            // @brief access the filters elements
            // @param x: the x-index, values in [0, width]
            // @param y: the y-index, values in [0, height]
            // @returns reference to the corresponding value
            virtual double & operator()(long x, long y);

            // @brief const-access the filters elements
            // @param x: the x-index, values in [0, width]
            // @param y: the y-index, values in [0, height]
            // @returns the corresponding value
            virtual double operator()(long x, long y) const;

            // @brief expose the filters values as one vector in row-major order
            // @returns const reference to the filters values
            const std::vector<double>& get_values() const;

            // pre-build functions, use with set_function

            // @brief return the identity function, the resulting filter with have a value of 1 in all components
            auto&& identity();

            // @brief shape into an ideal-lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            // @param cutoff_frequency: the radius of the passing region
            // @param pass_factor: the maximum value of the filter in the passing region
            // @param reject_factor: the minimum value of the filter in the rejecting region
            // @return lambda bindable via FrequencyDomainFilter::set_function
            auto&& ideal_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a gaussian lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            // @param cutoff_frequency: the standard deviation of the resulting gaussian curve
            // @param pass_factor: the maximum value of the filter in the passing region
            // @param reject_factor: the minimum value of the filter in the rejecting region
            // @return lambda bindable via FrequencyDomainFilter::set_function
            auto&& gaussian_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a butterworth lowpass filter of specified order. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            // @param cutoff_frequency: the maximum passed frequency
            // @param pass_factor: the maximum value of the filter in the passing region
            // @param reject_factor: the minimum value of the filter in the rejecting region
            // @return lambda bindable via FrequencyDomainFilter::set_function
            auto&& butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into an ideal-highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            // @param cutoff_frequency: the radius of the rejecting region
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& ideal_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a gaussian highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            // @param cutoff_frequency: the standard deviation of the gaussian curve modeling the rejecting region
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& gaussian_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a butterworth highpass filter of specified order. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            // @param cutoff_frequency: the maximum rejected frequency
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into an ideal bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            // @param lower_cutoff: the lowest frequency passed
            // @param high_cutoff: the highest frequency passed
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& ideal_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a gaussian bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            // @param lower_cutoff: the lowest frequency not attenuated
            // @param high_cutoff: the lowest frequency not attenuated
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& gaussian_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a butterworth bandpass filter of specified order. The resulting filter will pass frequencies inside the band interval and reject others
            // @param lower_cutoff: the lowest frequency not attenuated
            // @param high_cutoff: the lowest frequency not attenuated
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& butterworth_bandpass(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into an ideal bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            // @param lower_cutoff: the lowest frequency rejected
            // @param high_cutoff: the highest frequency rejected
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& ideal_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a gaussian bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            // @param lower_cutoff: the lowest frequency attenuated to the maximum degree
            // @param high_cutoff: the highest frequency attenuated to the maximum degree
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& gaussian_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            // @brief shape into a butterworth bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            // @param lower_cutoff: the lowest frequency attenuated to the maximum degree
            // @param high_cutoff: the highest frequency attenuated to the maximum degree
            // @param pass_factor: the maximum value in the passing region
            // @param reject_factor: the maximum value in the rejecting region
            // @returns lambda bindable via FrequencyDomainFilter::set_function
            auto&& butterworth_bandreject(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

        private:
            Vector2ui _size;
            bool _offset_symmetrical = true;
            Vector2ui _offset = {0,0};
            std::function<double(long, long)> _function = [](long x, long y) -> double {return 1;};

            double distance(long x, long y);

            template<typename T>
            static T project(T lower_bound, T upper_bound, T value)
            {
                return value * fabs(lower_bound - upper_bound) + std::min(lower_bound, upper_bound);
            }

            void initialize() const;
            mutable bool _values_initialized = false;
            mutable std::vector<double> _values;
    };
}

#include ".src/frequency_domain_filter.inl"