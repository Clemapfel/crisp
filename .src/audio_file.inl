// 
// Copyright 2021 Clemens Cords
// Created on 02.12.21 by clem (mail@clemens-cords.com)
//

#include <audio/audio_file.hpp>
#include <gpu_side/opengl_state.hpp>

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

    std::vector<float> AudioFile::get_samples() const
    {
        std::vector<float> data;
        size_t n = _buffer.getSampleCount();
        data.reserve(n);

        float max = std::numeric_limits<short>::min();
        float min = std::numeric_limits<short>::max();

        for (size_t i = 0; i < n; ++i)
        {
            data.push_back(_buffer.getSamples()[i]);
            min = std::min(min, data.back());
            max = std::max(max, data.back());
        }

        for (auto& s : data)
            s = ((s - min) / (max - min)) * 2 - 1;  // project into [-1, 1]

        return data;
    }
}