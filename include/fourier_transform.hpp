// 
// Copyright 2021 Clemens Cords
// Created on 09.08.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <image/multi_plane_image.hpp>
#include <image/grayscale_image.hpp>
#include <gpu_side/texture.hpp>

#include <complex>
#include <vector>

#include <fftw3.h>

namespace crisp
{
    /// @brief optimization mode of the transform
    enum FourierTransformMode : int
    {
        /// @brief prioritizes speed, high likelihood of rounding errors but suitable for real-time applications
        SPEED = 0,

        /// @brief recommended, heuristic DFT fit suboptimal, minimal rounding errors
        /// @note about 1.5 times slower than FourierTransformMode::SPEED
        BALANCED = 1,

        /// @brief unsuitable for real-time use, heuristic DFT optimal, minimal rounding errors
        /// @note about 10 times slower than FourierTransformMode::SPEED
        ACCURACY = 2
    };

    /// @brief fourier transform, values stored as magnitude and phase-angle
    /// @param mode: any of SPEED, BALANCED, ACCURACY, governs runtime performance
    template<FourierTransformMode Mode = BALANCED>
    class FourierTransform
    {
        template<typename>
        friend class FrequencyDomainFilter;

        using Value_t = typename std::conditional<Mode == SPEED, float, typename std::conditional<Mode == ACCURACY, long double, double>::type>::type;

        public:
            /// @brief default ctor
            FourierTransform() = default;

            /// @brief creates fourier transform from an image
            /// @param image: image the transform is constructed from
            template<typename Inner_t>
            void transform_from(const Image<Inner_t, 1>&);

            /// @brief transform back into an image, this does not modify the transform
            /// @returns resulting image
            template<typename Image_t>
            Image_t transform_to() const;

            /// @brief visualizes the transforms spectrum as an image
            /// @returns log(1+x)-scaled grayscale image of size m*n*2 where m, n size of the original transformed image
            [[nodiscard]] GrayScaleImage as_image() const;

            /// @brief visualize the transforms phase angles as an image
            /// @returns linearly scaled grayscale image of size m*n*2 where m, n size of the original transformed image
            [[nodiscard]] GrayScaleImage phase_angle_as_image() const;

            /// @brief visualize spectrum as texture
            /// @returns linearly scaled texture of size m*n*2 where m, n size of the original transformed image
            [[nodiscard]] Texture<float, 1> as_texture() const;

            /// @brief get the complex coefficient at the specified position, (0,0) being the top-left origin
            /// @param x: the row index, range [0, 2*m]
            /// @param y: the column index, range [0, 2*n]
            /// @returns coefficient as std::complex
            std::complex<Value_t> get_coefficient(size_t x, size_t y) const;

            /// @brief const-access the component of the spectrum at the specified position, (0,0) being the top-left origin
            /// @param x: the row index, range [0, 2*m]
            /// @param y: the column index, range [0, 2*n]
            /// @returns coefficients magnitude, float if in SPEED mode, double otherwise
            Value_t get_component(size_t x, size_t y) const;

            /// @brief access the component of the spectrum at the specified position, (0,0) being the top-left origin
            /// @param x: the row index, range [0, 2*m]
            /// @param y: the column index, range [0, 2*n]
            /// @returns reference to coefficients magnitude, float if in SPEED mode, double otherwise
            Value_t& get_component(size_t x, size_t y);

            /// @brief const-access the phase angle at the specified position, (0,0) being the top-left origin
            /// @param x: the row index, range [0, 2*m]
            /// @param y: the column index, range [0, 2*n]
            /// @returns angle in radians, float if in SPEED mode, double otherwise
            Value_t get_phase_angle(size_t x, size_t y) const;

            /// @brief access the phase angle at the specified position, (0,0) being the top-left origin
            /// @param x: the row index, range [0, 2*m]
            /// @param y: the column index, range [0, 2*n]
            /// @returns reference to phase angle in radians, float if in SPEED mode, double otherwise
            Value_t& get_phase_angle(size_t x, size_t y);

            /// @brief access the dc-component (the value at the spectrums center)
            /// @returns float if in SPEED mode, double otherwise
            /// @note equivalent to calling get_component(m/2, n/2) where m,n size of the transform
            Value_t get_dc_component() const;

            /// @brief get the size of the transforms
            /// @returns sf::Vector where .x is the width, .y the size of the transform
            Vector2ui get_size() const;

            /// @brief expose spectrum
            /// @returns reference to spectrum as vector
            std::vector<Value_t>& get_spectrum();

            /// @brief expose phase angle
            /// @returns reference to phase angle as vector
            std::vector<Value_t>& get_phase_angle();

        private:
            size_t to_index(size_t x, size_t y) const;

            Vector2ui _size;
            std::vector<Value_t> _spectrum,
                                 _phase_angle;

            Value_t _min_spectrum = 0, _max_spectrum = 1; // already log(1+x) scaled
    };

    template<FourierTransformMode Mode>
    using FourierTransform2D = FourierTransform<Mode>;

    template<FourierTransformMode Mode>
    class FourierTransform1D
    {
        using Value_t = typename std::conditional<Mode == SPEED, float, typename std::conditional<Mode == ACCURACY, long double, double>::type>::type;

        public:
            /// @brief default ctor
            FourierTransform1D() = default;

            /// @brief creates fourier transform from signal
            /// @param signal: 1d signal section to be transformed
            template<typename T>
            void transform_from(const std::vector<T>&);

            /// @brief creates fourier transform from signal
            /// @param begin: first sample
            /// @param n: number of samples
            template<typename T>
            void transform_from(const T* begin, size_t n);

            std::vector<float> as_signal() const;

            /// @brief transform back into signal
            /// @returns resulting signal
            std::vector<Value_t> transform_to() const;

            /// @brief get coefficient as complex number
            /// @param frequency: frequency component index, range [0, 2*m] where m the number of samples in the signal
            /// @returns complex number
            std::complex<Value_t> get_coefficient(size_t frequency) const;

            /// @brief const-access component of spectrum
            /// @param frequency: frequency component index, range [0, 2*m] where m the number of samples in the signal
            /// @returns component magnitude
            Value_t get_component(size_t frequency) const;

            /// @brief access component of spectrum
            /// @param frequency: frequency component index, range [0, 2*m] where m the number of samples in the signal
            /// @returns component magnitude
            Value_t& get_component(size_t frequency);

            /// @brief const-access phase angle of spectrum
            /// @param frequency: frequency component index, range [0, 2*m] where m the number of samples in the signal
            /// @returns component phase angle
            Value_t get_phase_angle(size_t frequency) const;

            /// @brief const-access phase angle of spectrum
            /// @param frequency: frequency component index, range [0, 2*m] where m the number of samples in the signal
            /// @returns component phase angle
            Value_t& get_phase_angle(size_t frequency);

            /// @brief get DC component value
            /// @returns component magnitude for frequency 0
            Value_t get_dc_component() const;

            /// @brief get size of spectrum
            /// @returns size
            size_t get_size() const;

            /// @brief expose spectrum
            /// @returns reference to spectrum as vector
            std::vector<Value_t>& get_spectrum();

            /// @brief expose phase angle
            /// @returns reference to phase angle as vector
            std::vector<Value_t>& get_phase_angle();

        private:
            size_t _size = 0;
            std::vector<Value_t> _spectrum,
                                 _phase_angle;

            Value_t _min_spectrum = 0, _max_spectrum = 1;
    };
}

#include ".src/fourier_transform_2d.inl"
#include ".src/fourier_transform_1d.inl"