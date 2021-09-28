// 
// Copyright 2021 Clemens Cords
// Created on 10.08.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<FourierTransformMode Mode>
    GrayScaleImage FourierTransform<Mode>::as_image() const
    {
        GrayScaleImage out;
        out.create(get_size().x(), get_size().y());

        size_t m = get_size().x(),
             n = get_size().y();

        for (size_t x = 0, i = 0; x < m; ++x)
        {
            for (size_t y = 0; y < n; ++y, ++i)
            {
                Value_t value = log(1 + _spectrum.at(i));

                if (_min_spectrum < 0)
                {
                    value += _min_spectrum;
                    value /= _max_spectrum;
                }
                else
                {
                    value -= _min_spectrum;
                    value /= (_max_spectrum + _min_spectrum);
                }

                out(x, y) = value;
            }
        }
        return out;
    }

    template<FourierTransformMode Mode>
    GrayScaleImage FourierTransform<Mode>::phase_angle_as_image() const
    {
        GrayScaleImage out;
        out.create(get_size().x(), get_size().y());

        size_t m = get_size().x(),
             n = get_size().y();

        for (size_t x = 0, i = 0; x < m; ++x)
        {
            for (size_t y = 0; y < n; ++y, ++i)
            {
                Value_t value = _phase_angle.at(i);
                value += M_PI;
                value /= 2 * M_PI;

                out(x, y) = value;
            }
        }
        return out;
    }

    template<FourierTransformMode Mode>
    std::complex<typename FourierTransform<Mode>::Value_t> FourierTransform<Mode>::get_coefficient(size_t x, size_t y) const
    {
        return std::polar(_spectrum.at(x * y), _phase_angle.at(x * y));
    }

    template<FourierTransformMode Mode>
    size_t FourierTransform<Mode>::to_index(size_t x, size_t y) const
    {
        return y + get_size().y()*x;
    }

    template<FourierTransformMode Mode>
    typename FourierTransform<Mode>::Value_t FourierTransform<Mode>::get_component(size_t x, size_t y) const
    {
        return _spectrum.at(to_index(x, y));
    }

    template<FourierTransformMode Mode>
    typename FourierTransform<Mode>::Value_t FourierTransform<Mode>::get_phase_angle(size_t x, size_t y) const
    {
        return _phase_angle.at(to_index(x, y));
    }

    template<FourierTransformMode Mode>
    typename FourierTransform<Mode>::Value_t& FourierTransform<Mode>::get_component(size_t x, size_t y)
    {
        return _spectrum.at(to_index(x, y));
    }

    template<FourierTransformMode Mode>
    typename FourierTransform<Mode>::Value_t& FourierTransform<Mode>::get_phase_angle(size_t x, size_t y)
    {
        return _phase_angle.at(to_index(x, y));
    }

    template<FourierTransformMode Mode>
    typename FourierTransform<Mode>::Value_t FourierTransform<Mode>::get_dc_component() const
    {
        return get_component(get_size().x() / 2, get_size().y() / 2);
    }

    template<FourierTransformMode Mode>
    Vector2ui FourierTransform<Mode>::get_size() const
    {
        return _size;
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform<Mode>::Value_t>& FourierTransform<Mode>::get_spectrum()
    {
        return _spectrum;
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform<Mode>::Value_t>& FourierTransform<Mode>::get_phase_angle()
    {
        return _phase_angle;
    }

    template<>
    template<typename Inner_t>
    void FourierTransform<SPEED>::transform_from(const Image<Inner_t, 1>& image_in)
    {
        size_t m = image_in.get_size().x() * 2;
        size_t n = image_in.get_size().y() * 2;

        _size = {m, n};

        auto* values = fftwf_alloc_complex(m*n);
        auto plan = fftwf_plan_dft_2d(m, n, values, values, FFTW_FORWARD, FFTW_ESTIMATE);

        bool dither = true;
        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                values[i][0] = static_cast<Value_t>(image_in(x, y)) * (dither ? 1 : -1);
                values[i][1] = 0;
                dither = not dither;
            }

            dither = not dither;
        }

        fftwf_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(m*n);
        _phase_angle.clear();
        _phase_angle.reserve(m*n);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::complex<float>(values[i][0], values[i][1]);
                auto magnitude = abs(f);

                _spectrum.emplace_back(magnitude);
                _phase_angle.emplace_back(arg(f));

                auto scaled = log(1 + magnitude);
                _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
                _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
            }
        }

        fftwf_destroy_plan(plan);
        fftwf_free(values);
    }

    template<>
    template<typename Image_t>
    Image_t FourierTransform<SPEED>::transform_to() const
    {
        Image_t image_out;
        image_out.create(get_size().x() / 2, get_size().y() / 2);

        size_t m = get_size().x();
        size_t n = get_size().y();

        auto* values = fftwf_alloc_complex(m*n);
        auto plan = fftwf_plan_dft_2d(m, n, values, values, FFTW_BACKWARD, FFTW_ESTIMATE);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::polar<float>(_spectrum.at(i), _phase_angle.at(i));
                values[i][0] = f.real();
                values[i][1] = f.imag();
            }
        }

        fftwf_execute(plan);

        bool dither = true; // inverted
        for (size_t y = 0, i = 0; y < n/2; ++y, i += m/2)
        {
            for (size_t x = 0; x < m/2; ++x, i++)
            {
                image_out(x, y) = static_cast<typename Image_t::Value_t>(values[i][0] / float(m * n) * (dither ? 1.f : -1.f));
                dither = not dither;
            }

            if (m/2 % 2 == 0)
                dither = not dither;
        }

        fftwf_destroy_plan(plan);
        fftwf_free(values);

        return image_out;
    }

    template<>
    template<typename Inner_t>
    void FourierTransform<BALANCED>::transform_from(const Image<Inner_t, 1>& image_in)
    {
        size_t m = image_in.get_size().x() * 2;
        size_t n = image_in.get_size().y() * 2;

        _size = {m, n};

        auto* values = fftw_alloc_complex(m*n);
        auto plan = fftw_plan_dft_2d(m, n, values, values, FFTW_FORWARD, FFTW_ESTIMATE);

        bool dither = true;
        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                values[i][0] = static_cast<double>(image_in(x, y)) * (dither ? 1.0 : -1.0);
                values[i][1] = 0;
                dither = not dither;
            }

            dither = not dither;
        }

        fftw_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(m*n);
        _phase_angle.clear();
        _phase_angle.reserve(m*n);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::complex<double>(values[i][0], values[i][1]);
                auto magnitude = abs(f);

                _spectrum.emplace_back(magnitude);
                _phase_angle.emplace_back(arg(f));

                auto scaled = log(1 + magnitude);
                _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
                _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
            }
        }

        fftw_destroy_plan(plan);
        fftw_free(values);
    }

    template<>
    template<typename Image_t>
    Image_t FourierTransform<BALANCED>::transform_to() const
    {
        Image_t image_out;
        image_out.create(get_size().x() / 2, get_size().y() / 2);

        size_t m = get_size().x();
        size_t n = get_size().y();

        auto* values = fftw_alloc_complex(m*n);
        auto plan = fftw_plan_dft_2d(m, n, values, values, FFTW_BACKWARD, FFTW_ESTIMATE_PATIENT);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::polar<double>(_spectrum.at(i), _phase_angle.at(i));
                values[i][0] = f.real();
                values[i][1] = f.imag();
            }
        }

        fftw_execute(plan);

        bool dither = true; // inverted
        for (size_t y = 0, i = 0; y < n/2; ++y, i += m/2)
        {
            for (size_t x = 0; x < m/2; ++x, i++)
            {
                image_out(x, y) = static_cast<typename Image_t::Value_t>(values[i][0] / double(m * n) * (dither ? 1.0 : -1.0));
                dither = not dither;
            }

            if (m/2 % 2 == 0)
                dither = not dither;
        }

        fftw_destroy_plan(plan);
        fftw_free(values);

        return image_out;
    }

    template<>
    template<typename Inner_t>
    void FourierTransform<ACCURACY>::transform_from(const Image<Inner_t, 1>& image_in)
    {
        size_t m = image_in.get_size().x() * 2;
        size_t n = image_in.get_size().y() * 2;

        _size = {m, n};

        auto* values = fftwl_alloc_complex(m*n);
        auto plan = fftwl_plan_dft_2d(m, n, values, values, FFTW_FORWARD, FFTW_ESTIMATE);

        bool dither = true;
        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                values[i][0] = static_cast<long double>(image_in(x, y)) * (dither ? 1. : -1.);
                values[i][1] = 0;
                dither = not dither;
            }

            dither = not dither;
        }

        fftwl_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(m*n);
        _phase_angle.clear();
        _phase_angle.reserve(m*n);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::complex<long double>(values[i][0], values[i][1]);
                auto magnitude = abs(f);

                _spectrum.emplace_back(magnitude);
                _phase_angle.emplace_back(arg(f));

                auto scaled = log(1 + magnitude);
                _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
                _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
            }
        }

        fftwl_destroy_plan(plan);
        fftwl_free(values);
    }

    template<>
    template<typename Image_t>
    Image_t FourierTransform<ACCURACY>::transform_to() const
    {
        Image_t image_out;
        image_out.create(get_size().x() / 2, get_size().y() / 2);

        size_t m = get_size().x();
        size_t n = get_size().y();

        auto* values = fftwl_alloc_complex(m*n);
        auto plan = fftwl_plan_dft_2d(m, n, values, values, FFTW_BACKWARD, FFTW_MEASURE);

        for (size_t y = 0, i = 0; y < n; ++y)
        {
            for (size_t x = 0; x < m; ++x, ++i)
            {
                auto f = std::polar<long double>(_spectrum.at(i), _phase_angle.at(i));
                values[i][0] = f.real();
                values[i][1] = f.imag();
            }
        }

        fftwl_execute(plan);

        bool dither = true; // inverted
        for (size_t y = 0, i = 0; y < n/2; ++y, i += m/2)
        {
            for (size_t x = 0; x < m/2; ++x, i++)
            {
                image_out(x, y) = values[i][0] / double(m * n) * (dither ? 1.f : -1.f);
                dither = not dither;
            }

            if (m/2 % 2 == 0)
                dither = not dither;
        }

        fftwl_destroy_plan(plan);
        fftwl_free(values);

        return image_out;
    }
}