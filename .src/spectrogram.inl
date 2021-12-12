// 
// Copyright 2021 Clemens Cords
// Created on 08.12.21 by clem (mail@clemens-cords.com)
//

#include <audio/spectrogram.hpp>
#include <fourier_transform.hpp>

#include <fftw3.h>

namespace crisp
{
    void Spectrogram::create_from(const AudioFile& audio, size_t window_size, float window_overlap, WindowType type)
    {
        create_from(audio, window_size, window_overlap, audio.get_n_samples(), type);
    }

    void Spectrogram::create_from(const AudioFile& audio, size_t window_size, float window_overlap, size_t n_windows, WindowType type)
    {
        auto n_samples = audio.get_n_samples();
        auto cols = std::min<size_t>(n_samples / (window_size * window_overlap), n_windows);
        _data.resize(window_size / 2, cols);

        auto signal = audio.get_samples();
        auto fourier = FourierTransform1D<SPEED>();

        size_t offset = 0;
        for (size_t i = 0; i < _data.cols(); ++i, offset += window_size * window_overlap)
        {
            std::vector<float> window;
            window.reserve(window_size / 2);

            for (size_t j = offset; j < offset + window_size; ++j)
            {
                float weight = 1;
                switch (type)
                {
                    case GAUSS:
                        weight = exp(-4 * pow(2 * ((j - offset) / float(n_samples)) - 1, 2));
                        break;
                    case HANNING:
                        weight = pow(cos(M_PI*((j - offset) / float(n_samples)) - (M_PI / 2)), 2);
                        break;
                    case BINARY:
                        weight = 1;
                        break;
                }

                window.push_back(weight * signal[j]);
            }

            fourier_transform(window);

            for (size_t j = 0; j < window.size(); ++j)
                _data(j, i) = window.at(j);
        }
    }

    void Spectrogram::fourier_transform(std::vector<float>& in)
    {
        auto* complex = fftwf_alloc_complex(in.size());

        auto plan = fftwf_plan_dft_r2c_1d(in.size(), &in[0], complex, FFTW_ESTIMATE);
        fftwf_execute(plan);

        float _min_spectrum = std::numeric_limits<float>::max();
        float _max_spectrum = std::numeric_limits<float>::min();

        in.resize(_data.rows());
        for (size_t i = 0; i < in.size(); ++i)
        {
            auto f = std::complex<float>(complex[i][0], complex[i][1]);
            in.at(i) = abs(f);

            _min_spectrum = std::min<float>(_min_spectrum, in.at(i));
            _max_spectrum = std::max<float>(_max_spectrum, in.at(i));
        }

        fftwf_destroy_plan(plan);
        fftwf_free(complex);

        auto low_boost = [](auto x){ return x;};//x * exp(-0.5*pow((3*x), 4));};
        auto normalize = [](auto x){ return log(1+x);};

        for (auto& e : in)
        {
            e = low_boost(e);
            e = (normalize(e) - normalize(_min_spectrum)) / (normalize(_max_spectrum) - normalize(_min_spectrum));
            e = clamp<float>(0, 1, e);
        }
    }

    GLNativeHandle Spectrogram::as_1d_texture_array() const
    {
        GLNativeHandle out;
        glGenTextures(1, &out);
        glBindTexture(GL_TEXTURE_1D_ARRAY, out);

        glTexStorage2D(GL_TEXTURE_1D_ARRAY, 1, GL_R32F, _data.rows(), _data.cols());
        glTexSubImage2D(
                GL_TEXTURE_1D_ARRAY,
                0,
                0,
                0,
                _data.rows(),
                _data.cols(),
                GL_RED,
                GL_FLOAT,
                &_data.data()[0]);

        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        return out;
    }

    GrayScaleImage Spectrogram::as_image() const
    {
        auto out = GrayScaleImage(_data.cols(), _data.rows());

        for (size_t x = 0; x < _data.rows(); ++x)
            for (size_t y = 0; y < _data.cols(); ++y)
                out(y, out.get_size().y() - +x) = _data(x, y);

        return out;
    }

    Vector2ui Spectrogram::get_size() const
    {
        return Vector2ui{size_t(_data.cols()), size_t(_data.rows())}; // sic
    }
}