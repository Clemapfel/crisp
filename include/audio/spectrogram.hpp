// 
// Copyright 2021 Clemens Cords
// Created on 08.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <audio/audio_file.hpp>
#include <Dense>

namespace crisp
{
    class Spectrogram
    {
        public:
            Spectrogram() = default;

            void create_from(const AudioFile&, size_t window_size, float window_overlap);
            void create_from(const AudioFile&, size_t window_size, float window_overlap, size_t first_window, size_t n_windows);

            GLNativeHandle as_1d_texture_array() const;
            GrayScaleImage as_image() const;

            Vector2ui get_size() const;

        private:
            void fourier_transform(std::vector<float>& in);
            float _min_spectrum = std::numeric_limits<float>::max();
            float _max_spectrum = std::numeric_limits<float>::min();

            Eigen::MatrixXf _data;
    };
}

#include ".src/spectrogram.inl"