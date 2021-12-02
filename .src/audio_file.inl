// 
// Copyright 2021 Clemens Cords
// Created on 02.12.21 by clem (mail@clemens-cords.com)
//

#include <audio/audio_file.hpp>
#include <gpu_side/state.hpp>

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

    size_t AudioFile::get_n_channels() const
    {
        return _buffer.getChannelCount();
    }

    float AudioFile::get_duration() const
    {
        return _buffer.getDuration().asSeconds();
    }

    size_t AudioFile::get_sample_rate() const
    {
        return _buffer.getSampleRate();
    }

    sf::Sound AudioFile::as_sound() const
    {
        return sf::Sound(_buffer);
    }

    std::vector<int16_t> AudioFile::get_samples() const
    {
        std::vector<int16_t> out;
        size_t n = _buffer.getSampleCount();
        out.reserve(n);

        for (size_t i = 0; i < n; ++i)
            out.push_back(_buffer.getSamples()[i]);

        return out;
    }

    GLNativeHandle AudioFile::as_1d_texture() const
    {

    }
}

