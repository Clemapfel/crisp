// 
// Copyright 2021 Clemens Cords
// Created on 08.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <audio/audio_file.hpp>
#include <Dense>

namespace crisp
{
    /// @brief sample audio signal in regular windows, then store fourier-transform of each window
    class Spectrogram
    {
        public:
            /// @brief default ctor
            Spectrogram() = default;

            /// @brief create from part of audio file
            /// @param audio_file
            /// @param window_size: number of samples in window, recommended: 1000 to 16 000
            /// @param window_overlap: overlap of adjacent windows, recommended: 0.05 to 1
            /// @param first_window: index of first sample
            /// @note windows are automatically weighted with a gaussian window function
            void create_from(const AudioFile&, size_t window_size, float window_overlap, size_t first_window, size_t n_windows);

            /// @brief create from entire audio file
            /// @param audio_file
            /// @param window_size: number of samples in window, recommended: 1000 to 16 000
            /// @param window_overlap: overlap of adjacent windows, recommended: 0.05 to 1
            void create_from(const AudioFile&, size_t window_size, float window_overlap);

            /// @brief transform into image
            /// @returns grayscale image of width (audio_file.get_n_samples() / (window_size * window_overlap)) and height (window_size / 2)
            GrayScaleImage as_image() const;

            /// @brief get size of spectrogram
            Vector2ui get_size() const;

        private:
            GLNativeHandle as_1d_texture_array() const;

            void fourier_transform(std::vector<float>& in);
            float _min_spectrum = std::numeric_limits<float>::max();
            float _max_spectrum = std::numeric_limits<float>::min();

            Eigen::MatrixXf _data;
    };
}

#include ".src/spectrogram.inl"