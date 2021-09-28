// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>::Iterator::Iterator(Image<InnerValue_t, N>* data, size_t x, size_t y)
        : _data(data), _x(x), _y(y), _size{_data->get_size().x(), _data->get_size().y()}
    {}

    template<typename InnerValue_t, size_t N>
    bool Image<InnerValue_t, N>::Iterator::operator==(Image<InnerValue_t, N>::Iterator& other) const
    {
        return _data == other._data and _x == other._x and _y == other._y;
    }

    template<typename InnerValue_t, size_t N>
    bool Image<InnerValue_t, N>::Iterator::operator!=(Iterator& other) const
    {
        return not operator==(other);
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Iterator & Image<InnerValue_t, N>::Iterator::operator++()
    {
        if (_x <_size.x() - 1)
            _x++;
        else if (_y < _size.y() - 1)
        {
            _x = 0;
            _y++;
        }
        else
        {
            _x++;
            _y++;
        }

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Iterator & Image<InnerValue_t, N>::Iterator::operator--()
    {
        if (_x > 0)
            _x--;
        else if (_y > 0)
        {
            _x =_size.x() - 1;
            _y--;
        }

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::Iterator::operator*() const
    {
        return _data->at(_x, _y);
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::Iterator & Image<InnerValue_t, N>::Iterator::operator=(Value_t value)
    {
        _data->at(_x, _y) = value;
        return *this;
    }

    template<typename InnerValue_t, size_t N>
    Image<InnerValue_t, N>::ConstIterator::ConstIterator(const Image<InnerValue_t, N>* data, size_t x, size_t y)
        : _data(data), _x(x), _y(y), _size{_data->get_size().x(), _data->get_size().y()}
    {}

    template<typename InnerValue_t, size_t N>
    bool Image<InnerValue_t, N>::ConstIterator::operator==(ConstIterator& other) const
    {
        return _data == other._data and _x == other._x and _y == other._y;
    }

    template<typename InnerValue_t, size_t N>
    bool Image<InnerValue_t, N>::ConstIterator::operator!=(ConstIterator& other) const
    {
        return not operator==(other);
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::ConstIterator & Image<InnerValue_t, N>::ConstIterator::operator++()
    {
        if (_x <_size.x() - 1)
            _x++;
        else if (_y < _size.y() - 1)
        {
            _x = 0;
            _y++;
        }
        else
        {
            _x++;
            _y++;
        }

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    typename Image<InnerValue_t, N>::ConstIterator & Image<InnerValue_t, N>::ConstIterator::operator--()
    {
        if (_x > 0)
            _x--;
        else if (_y > 0)
        {
            _x =_size.x() - 1;
            _y--;
        }

        return *this;
    }

    template<typename InnerValue_t, size_t N>
    const typename Image<InnerValue_t, N>::Value_t& Image<InnerValue_t, N>::ConstIterator::operator*() const
    {
        return _data->at(_x, _y);
    }
}