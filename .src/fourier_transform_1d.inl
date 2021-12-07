// 
// Copyright 2021 Clemens Cords
// Created on 10.08.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<FourierTransformMode Mode>
    std::complex<typename FourierTransform1D<Mode>::Value_t> FourierTransform1D<Mode>::get_coefficient(size_t frequency) const
    {
        return std::polar(_spectrum.at(frequency), _phase_angle.at(frequency));
    }

    template<FourierTransformMode Mode>
    typename FourierTransform1D<Mode>::Value_t FourierTransform1D<Mode>::get_component(size_t frequency) const
    {
        return _spectrum.at(frequency);
    }

    template<FourierTransformMode Mode>
    typename FourierTransform1D<Mode>::Value_t FourierTransform1D<Mode>::get_phase_angle(size_t frequency) const
    {
        return _phase_angle.at(frequency);
    }

    template<FourierTransformMode Mode>
    typename FourierTransform1D<Mode>::Value_t& FourierTransform1D<Mode>::get_component(size_t frequency)
    {
        return _spectrum.at(frequency);
    }

    template<FourierTransformMode Mode>
    typename FourierTransform1D<Mode>::Value_t& FourierTransform1D<Mode>::get_phase_angle(size_t frequency)
    {
        return _phase_angle.at(frequency);
    }

    template<FourierTransformMode Mode>
    typename FourierTransform1D<Mode>::Value_t FourierTransform1D<Mode>::get_dc_component() const
    {
        return get_component(0);
    }

    template<FourierTransformMode Mode>
    size_t FourierTransform1D<Mode>::get_size() const
    {
        return _size;
    }

    template<FourierTransformMode Mode>
    std::vector<float> FourierTransform1D<Mode>::as_signal() const
    {
        std::vector<float> out;
        out.reserve(_spectrum.size());

        for (size_t i = 0; i < _spectrum.size(); ++i)
        {
            float value = log(1 + _spectrum.at(i));
            value -= _min_spectrum;
            value /= (_max_spectrum - _min_spectrum);

            out.push_back(value);
        }

        return out;
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform1D<Mode>::Value_t>& FourierTransform1D<Mode>::get_spectrum()
    {
        return _spectrum;
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform1D<Mode>::Value_t>& FourierTransform1D<Mode>::get_phase_angle()
    {
        return _phase_angle;
    }

    template<FourierTransformMode Mode>
    template<typename T>
    void FourierTransform1D<Mode>::transform_from_real(const std::vector<T>& data)
    {
        transform_from_real(&data[0], data.size());
    }

    template<FourierTransformMode Mode>
    template<typename T>
    void FourierTransform1D<Mode>::transform_from_real(const T* begin, size_t n)
    {
        _size = n * 2;

        auto* in = fftw_alloc_real(_size);
        auto* out = fftw_alloc_complex(_size);

        auto plan = fftw_plan_dft_r2c_1d(_size, in, out, FFTW_ESTIMATE);

        bool dither = true;
        for (size_t i = 0; i < _size; ++i)
        {
            in[i] = (i < n ? static_cast<double>(*(begin + i)) : 0);// * (dither ? 1 : -1); // 0-padding
            dither = not dither;
        }

        fftw_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(_size);
        _phase_angle.clear();
        _phase_angle.reserve(_size);

        for (size_t i = 0; i < _size; ++i)
        {
            auto f = std::complex<float>(out[i][0], out[i][1]);
            auto magnitude = abs(f);

            _spectrum.emplace_back(magnitude);
            _phase_angle.emplace_back(arg(f));

            auto scaled = log(1 + magnitude);
            _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
            _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
        }

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);
    }

    template<FourierTransformMode Mode>
    template<typename T>
    void FourierTransform1D<Mode>::transform_from_complex(const std::vector<T>& data)
    {
        transform_from_complex(&data[0], data.size());
    }

    template<FourierTransformMode Mode>
    template<typename T>
    void FourierTransform1D<Mode>::transform_from_complex(const T* begin, size_t n)
    {
        _size = n * 2;

        auto* values = fftw_alloc_complex(_size);
        auto plan = fftw_plan_dft_1d(_size, values, values, FFTW_FORWARD, FFTW_ESTIMATE);

        bool dither = true;
        for (size_t i = 0; i < _size; ++i)
        {
            values[i][0] = (i < n ? static_cast<Value_t>(*(begin + i)) : 0);// * (dither ? 1 : -1); // 0-padding
            values[i][1] = 0;

            dither = not dither;
        }

        fftw_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(_size);
        _phase_angle.clear();
        _phase_angle.reserve(_size);

        for (size_t i = 0; i < _size; ++i)
        {
            auto f = std::complex<double>(values[i][0], values[i][1]);
            auto magnitude = abs(f);

            _spectrum.emplace_back(magnitude);
            _phase_angle.emplace_back(arg(f));

            auto scaled = log(1 + magnitude);
            _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
            _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
        }

        fftw_destroy_plan(plan);
        fftw_free(values);
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform1D<Mode>::Value_t> FourierTransform1D<Mode>::transform_to() const
    {
        assert(false);
        std::vector<Value_t> out;
        out.resize(_size / 2);

        auto* values = fftw_alloc_complex(_size);
        auto plan = fftw_plan_dft_1d(_size, values, values, FFTW_BACKWARD, FFTW_ESTIMATE);

        for (size_t i = 0; i < _size; ++i)
        {
            auto f = std::polar<double>(_spectrum.at(i), _phase_angle.at(i));
            values[i][0] = f.real();
            values[i][1] = f.imag();
        }

        fftw_execute(plan);

        bool dither = true;
        for (size_t i = 0; i < out.size(); ++i)
            out[i] = (values[i][0] / _size) * (dither ? 1.f : -1.f);

        return out;
    }
}