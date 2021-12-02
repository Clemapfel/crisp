// 
// Copyright 2021 Clemens Cords
// Created on 02.12.21 by clem (mail@clemens-cords.com)
//

#include <audio/audio_file.hpp>

namespace crisp
{
    AudioFile::AudioFile()
        : _buffer()
    {}

    void AudioFile::load(std::string path)
    {
        _buffer.loadFromFile(path);
    }

    void AudioFile::save(std::string path)
    {
        _buffer.saveToFile(path);
    }

    size_t AudioFile::get_n_samples() const
    {
        return _buffer.getSampleCount();
    }

    size_t AudioFile::get_sample_rate() const
    {
        return _buffer.getSampleRate();
    }

    sf::Sound AudioFile::as_sound() const
    {
        return sf::Sound(_buffer);
    }
}

