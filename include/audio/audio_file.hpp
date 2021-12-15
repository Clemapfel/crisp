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
            /// @brief default ctor
            AudioFile();

            /// @brief load from file
            /// @param path: absolute path
            /// @note: supported formats: .wav, .ogg
            void load(std::string path);

            /// @brief save to file
            /// @param path: absolute path
            /// @note: supported formats: .wav, .ogg
            void save(std::string path);

            /// @brief get number of samples in entire audio file
            /// @returns size_t
            size_t get_n_samples() const;

            /// @brief get number of channels
            /// @returns 1 for mono, 2 for stereo, >2 for surround
            size_t get_n_channels() const;

            /// @param get duration in seconds
            /// @returns number of seconds
            float get_duration() const;

            /// @param get sample rate
            /// @returns number of samples per second
            size_t get_sample_rate() const;

            /// @param normalize signal into range [-1, 1] and return as vector
            /// @returns vector of floats
            [[nodiscard]] std::vector<float> get_samples() const;

        protected:
            /// @brief transform to playable sound
            /// @returns sf::Sound
            sf::Sound as_sound() const;

        private:
            sf::SoundBuffer _buffer;
    };
}

#include ".src/audio_file.inl"