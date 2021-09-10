// 
// Copyright 2021 Clemens Cords
// Created on 09.08.21 by clem (mail@clemens-cords.com)
//

#include <frequency_domain_filter.hpp>
#include ".src/common.inl"

namespace crisp
{
    inline FrequencyDomainFilter::FrequencyDomainFilter(size_t width, size_t height)
        : _size{width, height}
    {}

    template<FourierTransformMode Mode>
    inline FrequencyDomainFilter::FrequencyDomainFilter(const FourierTransform<Mode>& transform)
    {
        _size = Vector2ui{transform.get_size().x(), transform.get_size().y()};
    }

    inline const std::vector<double> & FrequencyDomainFilter::get_values() const
    {
        if (not _values_initialized)
            initialize();

        return _values;
    }

    inline void FrequencyDomainFilter::set_function(std::function<double(size_t, size_t)>&& f)
    {
        _function = f;
        _values_initialized = false;
    }

    inline void FrequencyDomainFilter::set_offset(size_t x_dist_from_center, size_t y_dist_from_center, bool force_symmetry)
    {
        _offset_symmetrical = force_symmetry;
        _offset = Vector2ui{x_dist_from_center, y_dist_from_center};
        _values_initialized = false;
    }

    inline Vector2ui FrequencyDomainFilter::get_size() const
    {
        return _size;
    }

    inline void FrequencyDomainFilter::set_size(Vector2ui size)
    {
        _size = size;
        _values_initialized = false;
    }

    inline void FrequencyDomainFilter::initialize() const
    {
        _values.clear();
        _values.reserve(_size.x() * _size.y());

        for (size_t x = 0, i = 0; x <_size.x(); ++x)
            for (size_t y = 0; y < _size.y(); ++y, ++i)
                _values.emplace_back(_function(std::max<int>(x - _offset.x(), 0), std::max<int>(y - _offset.y(), 0)) * (_offset_symmetrical ? _function(std::min(x + _offset.x(), get_size().x()), std::min(y + _offset.y(), get_size().y())) : 1));

        _values_initialized = true;
    }

    inline double FrequencyDomainFilter::distance(size_t x_in, size_t y_in)
    {
        auto x = x_in -_size.x() / 2.f;
        auto y = y_in - _size.y() / 2.f;
        return sqrt(x*x + y*y);
    }

    inline auto && FrequencyDomainFilter::identity()
    {
        static auto f =[](int x, int y) -> double {return 1;};
        
        return f;
    }

    // IDEAL
    inline auto && FrequencyDomainFilter::ideal_lowpass(double cutoff_frequency, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, pass_factor, reject_factor](int x, int y) -> double {
            return distance(x, y) < cutoff_frequency ? pass_factor : reject_factor;
        };
        
        return f;
    }

    inline auto && FrequencyDomainFilter::ideal_highpass(double cutoff_frequency, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, pass_factor, reject_factor](int x, int y) -> double {return distance(x, y) < cutoff_frequency ? reject_factor : pass_factor;};
        
        return f;
    }

    inline auto&& FrequencyDomainFilter::ideal_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor, double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            return dist > lower_cutoff and dist < higher_cutoff ? reject_factor : pass_factor;
        };
        
        return f;
    }

    inline auto&& FrequencyDomainFilter::ideal_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor, double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            return dist > lower_cutoff and dist < higher_cutoff ? pass_factor : reject_factor;
        };
        
        return f;
    }

    // GAUSSIAN

    inline auto && FrequencyDomainFilter::gaussian_lowpass(double cutoff_frequency, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, pass_factor, reject_factor](int x, int y) -> double {

            auto dist = distance(x, y);
            auto res = exp(-0.5 * pow(distance(x, y) / cutoff_frequency, 2));
            return project<double>(reject_factor, pass_factor, res);
        };
        
        return f;
    }

    inline auto && FrequencyDomainFilter::gaussian_highpass(double cutoff_frequency, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, pass_factor, reject_factor](int x, int y) -> double {

            auto dist = distance(x, y);
            auto res = exp(-0.5 * pow(distance(x, y) / cutoff_frequency, 2));
            return 1 - project<double>(pass_factor, reject_factor, res);
        };
        
        return f;
    }

    inline auto&& FrequencyDomainFilter::gaussian_bandreject(double lower_cutoff, double higher_cutoff, double pass_factor,
                                                    double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            auto width = higher_cutoff - lower_cutoff;
            auto center = lower_cutoff + width/2;
            auto res = 1 - exp(-1*pow((dist*dist - center*center) / (dist*width), 2));
            return project<double>(reject_factor, pass_factor, res);
        };
        
        return f;
    }

    inline auto&& FrequencyDomainFilter::gaussian_bandpass(double lower_cutoff, double higher_cutoff, double pass_factor,
                                                    double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            auto width = higher_cutoff - lower_cutoff;
            auto center = lower_cutoff + width/2;
            auto res = exp(-1*pow((dist*dist - center*center) / (dist*width), 2));
            return project<double>(reject_factor, pass_factor, res);
        };
        
        return f;
    }

    // BUTTERWORTH

    inline auto && FrequencyDomainFilter::butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, order, pass_factor, reject_factor](int x, int y) -> double {
            auto res = 1 / (1 + pow(distance(x, y) / cutoff_frequency, 2 * order));
            return project<double>(reject_factor, pass_factor, res);
        };
        
        return f;
    }


    inline auto && FrequencyDomainFilter::butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor, double reject_factor)
    {
        static auto f =[this, cutoff_frequency, order, pass_factor, reject_factor](int x, int y) -> double {
            auto res = (1 / (1 + pow(distance(x, y) / cutoff_frequency, 2 * order)));
            return 1 - project<double>(pass_factor, reject_factor, res);
        };
        
        return f;
    }

    inline auto && FrequencyDomainFilter::butterworth_bandreject(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor, double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, order, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            auto width = higher_cutoff - lower_cutoff;
            auto center = lower_cutoff + width/2;

            auto res = 1 / (1 + pow((dist*width)/(dist*dist-center*center), 2*order));
            return project<double>(reject_factor, pass_factor, res);
        };
        
        return f;
    }

    inline auto && FrequencyDomainFilter::butterworth_bandpass(double lower_cutoff, double higher_cutoff, size_t order, double pass_factor, double reject_factor)
    {
        static auto f =[this, lower_cutoff, higher_cutoff, order, pass_factor, reject_factor](int x, int y) -> double {
            auto dist = distance(x, y);
            auto width = higher_cutoff - lower_cutoff;
            auto center = lower_cutoff + width/2;

            auto res = 1 / (1 + pow((dist*width)/(dist*dist-center*center), 2*order));
            return project<double>(reject_factor, pass_factor, 1 - res);
        };
        
        return f;
    }

    template<FourierTransformMode Mode>
    void FrequencyDomainFilter::apply_to(FourierTransform<Mode>& fourier) const
    {
        assert(fourier.get_size().x() == get_size().x() and fourier.get_size().y() == get_size().y());

        if (not _values_initialized)
            initialize();

        for (size_t x = 0, i = 0; x < fourier.get_size().x(); ++x)
            for (size_t y = 0; y < fourier.get_size().y(); ++y, ++i)
                fourier.get_component(x, y) *= _values.at(i);
    }

    inline FrequencyDomainFilter FrequencyDomainFilter::operator+(const FrequencyDomainFilter& other) const
    {
        assert(_size == other._size);

        auto out = FrequencyDomainFilter(_size.x(), _size.y());
        out._values.clear();

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
                out._values.push_back(_values.at(i) + other._values.at(i));

        return out;
    }

    inline FrequencyDomainFilter FrequencyDomainFilter::operator-(const FrequencyDomainFilter& other) const
    {
        assert(_size == other._size);

        auto out = FrequencyDomainFilter(_size.x(), _size.y());
        out._values.clear();

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
                out._values.push_back(_values.at(i) - other._values.at(i));

        return out;
    }

    inline FrequencyDomainFilter FrequencyDomainFilter::operator*(const FrequencyDomainFilter& other) const
    {
        assert(_size == other._size);

        auto out = FrequencyDomainFilter(_size.x(), _size.y());
        out._values.clear();

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
                out._values.push_back(_values.at(i) * other._values.at(i));

        return out;
    }

    inline FrequencyDomainFilter FrequencyDomainFilter::operator/(const FrequencyDomainFilter& other) const
    {
        assert(_size == other._size);

        auto out = FrequencyDomainFilter(_size.x(), _size.y());
        out._values.clear();

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
                out._values.push_back(_values.at(i) / other._values.at(i));

        return out;
    }

    inline FrequencyDomainFilter& FrequencyDomainFilter::operator+=(const FrequencyDomainFilter& other)
    {
        assert(_size == other._size);

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
            _values.at(i) = _values.at(i) + other._values.at(i);

        return *this;
    }

    inline FrequencyDomainFilter& FrequencyDomainFilter::operator-=(const FrequencyDomainFilter& other)
    {
        assert(_size == other._size);

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
            _values.at(i) = _values.at(i) - other._values.at(i);

        return *this;
    }

    inline FrequencyDomainFilter& FrequencyDomainFilter::operator*=(const FrequencyDomainFilter& other)
    {
        assert(_size == other._size);

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
            _values.at(i) = _values.at(i) * other._values.at(i);

        return *this;
    }

    inline FrequencyDomainFilter& FrequencyDomainFilter::operator/=(const FrequencyDomainFilter& other)
    {
        assert(_size == other._size);

        if (not _values_initialized)
            initialize();

        if (not other._values_initialized)
            other.initialize();

        for (size_t i = 0; i < _values.size(); ++i)
            _values.at(i) = _values.at(i) / other._values.at(i);

        return *this;
    }

    inline double& FrequencyDomainFilter::operator()(size_t x, size_t y)
    {
        if (not _values_initialized)
            initialize();

        return _values.at(y + _size.y() * x);
    }

    inline double FrequencyDomainFilter::operator()(size_t x, size_t y) const
    {
        if (not _values_initialized)
            initialize();

        return _values.at(y + _size.y() * x);
    }
}