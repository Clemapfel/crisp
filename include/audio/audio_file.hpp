// 
// Copyright 2021 Clemens Cords
// Created on 02.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <SFML/Audio.hpp>

#include <array>

#include <gpu_side/native_handle.hpp>

namespace crisp
{
    /// @brief audio capture where entire signal is available at the time of loading
    class AudioFile
    {
        public:
            AudioFile();

            void load(std::string path);
            void save(std::string path);

            size_t get_n_samples() const;
            size_t get_n_channels() const;

            /// @returns duration in seconds
            float get_duration() const;

            /// @returns number of samples per second
            size_t get_sample_rate() const;

            sf::Sound as_sound() const;

            GLNativeHandle as_1d_texture() const;

            [[nodiscard]] std::vector<int16_t> get_samples() const;

        private:
            sf::SoundBuffer _buffer;
    };
}

#include ".src/audio_file.inl"