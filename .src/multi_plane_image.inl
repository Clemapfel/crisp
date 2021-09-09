// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>::Image(size_t width, size_t height, Value_t init)
    {
        create(width, height, init);
    }

    template<typename InnerValue_t, size_t N>
    void Image<InnerValue_t, N>::create(size_t width, size_t height, Value_t init)
    {
        _data.resize(width, height);
        _data.setConstant(init);
    }

    template<typename InnerValue_t, size_t N>
    const typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::at(size_t x, size_t y) const
    {
        if (x < 0  or x >= _data.rows() or y < 0 or y >= _data.cols())
            throw std::out_of_range("index out of range when trying to access pixel via Image::at");

        return _data(x, y);
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::at(size_t x, size_t y)
    {
        if (x < 0  or x >= _data.rows() or y < 0 or y >= _data.cols())
            throw std::out_of_range("index out of range when trying to access pixel via Image::at");

        return _data(x, y);
    }


    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Value_t Image<InnerValue_t, N>::get_pixel_out_of_bounds(int x, int y) const
    {
        assert(not (x >= 0 and x < _data.rows() and y >= 0 and y < _data.cols()));

        switch (_padding_type)
        {
            case ZERO:
                return Value_t(InnerValue_t(0));
            case ONE:
                return Value_t(InnerValue_t(1));
            case REPEAT:
            {
                int x_mod = x % int(get_size().x());
                int y_mod = y % int(get_size().y());

                if (x_mod < 0)
                    x_mod += get_size().x();

                if (y_mod < 0)
                    y_mod += get_size().y();

                return at(x_mod, y_mod);
            }
            case MIRROR:
            {
                int new_x = x % (get_size().x() - 1);
                if (x < 0)
                    new_x = abs(new_x);
                else if (x >= get_size().x())
                    new_x = get_size().x() - 1 - new_x;

                int new_y = y % (get_size().y() - 1);
                if (y < 0)
                    new_y = abs(new_y);
                else if (y >= get_size().y())
                    new_y = get_size().y() - 1 - new_y;

                return at(new_x, new_y);
            }
            case STRETCH:
            {
                int new_x = x;
                if (x < 0)
                    new_x = 0;
                if (x >= get_size().x())
                    new_x = get_size().x() - 1;

                int new_y = y;
                if (y < 0)
                    new_y = 0;
                if (y >= get_size().y())
                    new_y = get_size().y() - 1;

                return at(new_x, new_y);
            }
            default:
                return Value_t(InnerValue_t(0));
        }
    }

    template<typename InnerValue_t, size_t N>
    const typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::operator()(int x, int y) const
    {
        if (x < 0  or x >= _data.rows() or y < 0 or y >= _data.cols())
        {
            _dummy_padding_reference = get_pixel_out_of_bounds(x, y);
            return _dummy_padding_reference;
        }
        else
            return _data(x, y);
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::operator()(int x, int y)
    {
        if (x < 0  or x >= _data.rows() or y < 0 or y >= _data.cols())
        {
            _dummy_padding_reference = get_pixel_out_of_bounds(x, y);
            return _dummy_padding_reference;
        }
        else
            return _data(x, y);
    }

    template<typename InnerValue_t, size_t N>
    Vector2ui Image<InnerValue_t, N>::get_size() const
    {
        return Vector2ui({size_t(_data.rows()), size_t(_data.cols())});
    }

    template<typename InnerValue_t, size_t N>
    void Image<InnerValue_t, N>::set_padding_type(PaddingType type)
    {
        _padding_type = type;
    }

    template<typename InnerValue_t, size_t N>
    PaddingType Image<InnerValue_t, N>::get_padding_type() const
    {
        return _padding_type;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N> Image<InnerValue_t, N>::operator+(const Image<InnerValue_t, N>& other) const
    {
        assert(get_size() == other.get_size());

        Image<InnerValue_t, N> out;

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                out(x, y) = at(x, y) + other.at(x, y);

        return out;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N> Image<InnerValue_t, N>::operator-(const Image<InnerValue_t, N>& other) const
    {
        assert(get_size() == other.get_size());

        Image<InnerValue_t, N> out;

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                out(x, y) = at(x, y) - other.at(x, y);

        return out;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N> Image<InnerValue_t, N>::operator*(const Image<InnerValue_t, N>& other) const
    {
        assert(get_size() == other.get_size());

        Image<InnerValue_t, N> out;

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                out(x, y) = at(x, y) * other.at(x, y);

        return out;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N> Image<InnerValue_t, N>::operator/(const Image<InnerValue_t, N>& other) const
    {
        assert(get_size() == other.get_size());

        Image<InnerValue_t, N> out;

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                out(x, y) = at(x, y) / other.at(x, y);

        return out;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>& Image<InnerValue_t, N>::operator+=(const Image<InnerValue_t, N>& other)
    {
        assert(get_size() == other.get_size());

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                this->at(x, y) += other.at(x, y);

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>& Image<InnerValue_t, N>::operator-=(const Image<InnerValue_t, N>& other)
    {
        assert(get_size() == other.get_size());

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                this->at(x, y) -= other.at(x, y);

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>& Image<InnerValue_t, N>::operator*=(const Image<InnerValue_t, N>& other)
    {
        assert(get_size() == other.get_size());

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                this->at(x, y) *= other.at(x, y);

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>& Image<InnerValue_t, N>::operator/=(const Image<InnerValue_t, N>& other)
    {
        assert(get_size() == other.get_size());

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.cols(); ++x)
                this->at(x, y) /= other.at(x, y);
        return *this;
    }

    template<typename InnerValue_t, size_t N>
    template<size_t PlaneIndex>
    Image<InnerValue_t, 1>  Image<InnerValue_t, N>::get_nths_plane() const
    {
        static_assert(PlaneIndex < N, "Please specify an plane index less than N");

        Image<InnerValue_t, 1> out;
        out.create(_data.rows(), _data.cols());

        for (long y = 0; y < _data.cols(); ++y)
            for (long x = 0; x < _data.rows(); ++x)
                out(x, y) = _data(x, y).at(PlaneIndex);

        return out;
    }


    template<typename InnerValue_t, size_t N>
    auto Image<InnerValue_t, N>::begin()
    {
        return Iterator(this, 0, 0);
    }
    
    template<typename InnerValue_t, size_t N>
    auto Image<InnerValue_t, N>::begin() const
    {
        return ConstIterator(this, 0, 0);
    }
    
    template<typename InnerValue_t, size_t N>
    auto Image<InnerValue_t, N>::end()
    {
        return Iterator(this, _data.rows(), _data.cols());
    }
    
    template<typename InnerValue_t, size_t N>
    auto Image<InnerValue_t, N>::end() const
    {
        return ConstIterator(this, _data.rows(), _data.cols());
    }
}

