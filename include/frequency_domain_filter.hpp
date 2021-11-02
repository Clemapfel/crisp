// 
// Copyright 2021 Clemens Cords
// Created on 09.08.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <fourier_transform.hpp>
#include <gpu_side/is_gpu_side.hpp>

namespace crisp
{
    template<typename side = CPU_SIDE>
    class FrequencyDomainFilter;

    /// @brief filter intended to be applied to frequency-domain transforms such as the fourier transform
    template<>
    class FrequencyDomainFilter<CPU_SIDE>
    {
        public:
            /// @brief construct the filter of a specified size
            /// @param width: the x-dimension of the spectrum the filter will be applied to
            /// @param height: the y-dimensions of the spectrum the filter will be applied to
            FrequencyDomainFilter(size_t width, size_t height);

            /// @brief construct filter of the same size as the spectrum
            /// @param spectrum
            template<FourierTransformMode Mode>
            FrequencyDomainFilter(const FourierTransform<Mode>&);

            /// @brief multiply the filter with a fourier spectrum
            /// @tparam gpu_side: use hardware acceleration
            /// @tparam mode: performance mode of transform, usually auto deduced
            /// @param spectrum: the spectrum to be modified
            template<FourierTransformMode Mode>
            void apply_to(FourierTransform<Mode>&) const;

            /// @brief resize the filter
            /// @param size: vector where .x is the x-dimensions and .y the y-dimensions of the spectrum the filter will be applied to
            void set_size(Vector2ui);

            /// @brief get the filters dimensions
            /// @returns vector where .x is the width, .y the height of the filter
            Vector2ui get_size() const;

            /// @brief move the center of the function relative to the filters center origin
            /// @param x_dist_from_center: x-distance, values in [-0.5, +0.5]
            /// @param y_dist_from_center: y-distance, values in [-0.5, +0.5]
            /// @param force_symmetry: should the filter be mirrored across it's origin
            void set_offset(float x_dist_from_center, float y_dist_from_center, bool force_symmetry = true);

            /// @brief normalize all values in the filter to specified interval
            /// @param min: minimum value in filter
            /// @param max: maximum value in filter
            void normalize(double min = 0, double max = 1);

            /// @brief element-wise add one filter to another
            /// @param : other filter
            /// @returns resulting filter
            FrequencyDomainFilter operator+(const FrequencyDomainFilter&) const;

            /// @brief element-wise subtract one filter from another
            /// @param : other filter
            /// @returns resulting filter
            FrequencyDomainFilter operator-(const FrequencyDomainFilter&) const;

            /// @brief element-wise add multiply one filter with another
            /// @param : other filter
            /// @returns resulting filter
            FrequencyDomainFilter operator*(const FrequencyDomainFilter&) const;

            /// @brief element-wise divide one filter by another
            /// @param : other filter
            /// @returns resulting filter
            FrequencyDomainFilter operator/(const FrequencyDomainFilter&) const;

            /// @brief assign the element-wise addition with another filter
            /// @param : other filter
            /// @returns reference to self after assignment
            FrequencyDomainFilter& operator+=(const FrequencyDomainFilter&);

            /// @brief assign the element-wise subtraction from another filter
            /// @param : other filter
            /// @returns reference to self after assignment
            FrequencyDomainFilter& operator-=(const FrequencyDomainFilter&);

            /// @brief assign the element-wise multiplication with another filter
            /// @param : other filter
            /// @returns reference to self after assignment
            FrequencyDomainFilter& operator*=(const FrequencyDomainFilter&);

            /// @brief assign the element-wise division by another filter
            /// @param : other filter
            /// @returns reference to self after assignment
            FrequencyDomainFilter& operator/=(const FrequencyDomainFilter&);

            /// @brief access the filters elements
            /// @param x: the x-index, values in [0, width]
            /// @param y: the y-index, values in [0, height]
            /// @returns reference to the corresponding value
            double & operator()(size_t x, size_t y);

            /// @brief const-access the filters elements
            /// @param x: the x-index, values in [0, width]
            /// @param y: the y-index, values in [0, height]
            /// @returns the corresponding value
            double operator()(size_t x, size_t y) const;

            /// @brief access the filters elements with bounds checking
            /// @param x: the x-index, values in [0, width]
            /// @param y: the y-index, values in [0, height]
            /// @returns reference to the corresponding value
            double & at(size_t x, size_t y);

            /// @brief const-access the filters elements with bounds checking
            /// @param x: the x-index, values in [0, width]
            /// @param y: the y-index, values in [0, height]
            /// @returns the corresponding value
            double at(size_t x, size_t y) const;

            /// @brief expose the filters values as one vector in row-major order
            /// @returns const reference to the filters values
            const std::vector<double>& get_values() const;

            /// @brief transform spectrum into a 1-plane image of the same size, each coefficient is log-scaled and normalized into [0, 1]
            /// @returns 1-plane image of specified type
            template<typename Image_t>
            Image_t as_image() const;

            /// @brief identity filter
            /// @returns filter where all elements are 1
            void as_identity();

            /// @brief shape into an ideal-lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_ideal_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_gaussian_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth lowpass filter of specified order. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal-highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth highpass filter of specified order. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth bandpass filter of specified order. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_bandpass(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_bandreject(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

        private:
            Vector2ui _size;
            bool _offset_symmetrical = true;
            Vector2f _offset = {0,0};
            std::function<double(size_t, size_t)> _function;

            double distance(size_t x, size_t y);

            void initialize() const;
            mutable bool _values_initialized = false;
            mutable std::vector<double> _values;
    };

    /// @brief gpu-side version of frequency domain filter, less flexible but much faster
    template<>
    class FrequencyDomainFilter<GPU_SIDE>
    {
        public:
            /// @brief construct the filter of a specified size
            /// @param width: the x-dimension of the spectrum the filter will be applied to
            /// @param height: the y-dimensions of the spectrum the filter will be applied to
            FrequencyDomainFilter(size_t width, size_t height);

            /// @brief construct filter of the same size as the spectrum
            /// @param spectrum
            template<FourierTransformMode Mode>
            FrequencyDomainFilter(const FourierTransform<Mode>&);

            /// @brief multiply the filter with a fourier spectrum
            /// @tparam gpu_side: use hardware acceleration
            /// @tparam mode: performance mode of transform, usually auto deduced
            /// @param spectrum: the spectrum to be modified
            template<FourierTransformMode Mode>
            void apply_to(FourierTransform<Mode>&) const;

            /// @brief set the filters offset, symmetry is automatically enforced
            /// @param x_dist_from_center: x offset
            /// @param y_dist_from_center: y offset
            void set_offset(double x_dist_from_center, double y_dist_from_center);

            /// @brief identity filter
            /// @returns filter where all elements are 1
            void as_identity();

            /// @brief shape into an ideal-lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_ideal_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian lowpass filter. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_gaussian_lowpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth lowpass filter of specified order. The resulting filter will pass lower frequencies closer to the center of the spectrum and reject higher frequencies
            /// @param cutoff_frequency: radius of the passing region, in [0, 0.5]
            /// @param pass_factor: maximum value of the filter in the passing region
            /// @param reject_factor: minimum value of the filter in the rejecting region
            void as_butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal-highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian highpass filter. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_highpass(double cutoff_frequency, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth highpass filter of specified order. The resulting filter will reject lower frequencies closer to the center of the spectrum and pass higher frequencies
            /// @param cutoff_frequency: radius of the rejecting region, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian bandpass filter. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth bandpass filter of specified order. The resulting filter will pass frequencies inside the band interval and reject others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_bandpass(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into an ideal bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_ideal_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a gaussian bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_gaussian_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor = 1, double reject_factor = 0);

            /// @brief shape into a butterworth bandreject filter. The resulting filter will reject frequencies inside the band and pass others
            /// @param lower_cutoff: lower bound, in [0, 0.5]
            /// @param high_cutoff: upper bound, in [0, 0.5]
            /// @param pass_factor: maximum value in the passing region
            /// @param reject_factor: maximum value in the rejecting region
            void as_butterworth_bandreject(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor = 1, double reject_factor = 0);

        private:
            GLNativeHandle _program;

            double _cutoff_a, _cutoff_b;
            double _pass_factor, _reject_factor;
            Vector2f _offset = Vector2f{0, 0};
            size_t _order = 0;

            Vector2f _size;
    };
}

#include ".src/frequency_domain_filter.inl"
#include ".src/frequency_domain_filter_gpu.inl"