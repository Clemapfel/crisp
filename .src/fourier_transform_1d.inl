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
    void FourierTransform1D<Mode>::transform_from(const std::vector<T>& data)
    {
        _size = data.size() * 2;

        auto* values = fftwf_alloc_complex(_size);
        auto plan = fftwf_plan_dft_1d(_size, values, values, FFTW_FORWARD, FFTW_ESTIMATE);

        for (size_t i = 0; i < _size; ++i)
        {
            values[i][0] = i < data.size() ? static_cast<Value_t>(data[i]) : 0; // 0-padding
            values[i][1] = 0;
        }

        fftwf_execute(plan);

        _min_spectrum = std::numeric_limits<Value_t>::max();
        _max_spectrum = std::numeric_limits<Value_t>::min();

        _spectrum.clear();
        _spectrum.reserve(_size);
        _phase_angle.clear();
        _phase_angle.reserve(_size);

        for (size_t i = 0; i < _size; ++i)
        {
            auto f = std::complex<float>(values[i][0], values[i][1]);
            auto magnitude = abs(f);

            _spectrum.emplace_back(magnitude);
            _phase_angle.emplace_back(arg(f));

            auto scaled = log(1 + magnitude);
            _min_spectrum = std::min<Value_t>(_min_spectrum, scaled);
            _max_spectrum = std::max<Value_t>(_max_spectrum, scaled);
        }

        fftwf_destroy_plan(plan);
        fftwf_free(values);
    }

    template<FourierTransformMode Mode>
    std::vector<typename FourierTransform1D<Mode>::Value_t> FourierTransform1D<Mode>::transform_to() const
    {
        std::vector<Value_t> out;
        out.resize(_size / 2);

        auto* values = fftwf_alloc_complex(_size);
        auto plan = fftwf_plan_dft_1d(_size, values, values, FFTW_BACKWARD, FFTW_ESTIMATE);

        for (size_t i = 0; i < _size; ++i)
        {
            auto f = std::polar<float>(_spectrum.at(i), _phase_angle.at(i));
            values[i][0] = f.real();
            values[i][1] = f.imag();
        }

        fftwf_execute(plan);

        for (size_t i = 0; i < out.size(); ++i)
            out[i] = values[i][0] / _size;

        return out;
    }
}