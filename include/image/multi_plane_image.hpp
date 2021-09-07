// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <image/padding_type.hpp>

#include <type_traits>

namespace crisp
{
    // an image that lives in ram and is operated upon by the cpu
    template<typename InnerValue_t, size_t N = 1>
    class Image
    {
        class Iterator;
        class ConstIterator;

        public:
            using Value_t = Vector<InnerValue_t, N>;
            using Image_t = Image<InnerValue_t, N>;

            // @brief ctors
            Image() = default;
            Image(size_t width, size_t height, Value_t init = Value_t(InnerValue_t(0)));

            // @brief create as given size with 1 value
            void create(size_t width, size_t height, Value_t init = Value_t(InnerValue_t(0)));

            // @brief access pixel or padding if out of range
            Value_t& operator()(int x, int y);
            Value_t operator()(int x, int y) const;

            // @brief access pixel with bounds checking
            Value_t at(size_t x, size_t y) const;
            Value_t& at(size_t x, size_t y);

            // @brief get number of pixels
            Vector2ui get_size() const;

            // @brief specify the padding type, STRETCH by default
            void set_padding_type(PaddingType);

            // @brief access the padding type
            PaddingType get_padding_type() const;

            // @brief image-image arithmetics
            Image_t operator+(const Image_t&) const;
            Image_t operator-(const Image_t&) const;
            Image_t operator*(const Image_t&) const;
            Image_t operator/(const Image_t&) const;

            // @brief image-image assignment
            Image_t& operator+=(const Image_t&);
            Image_t& operator-=(const Image_t&);
            Image_t& operator*=(const Image_t&);
            Image_t& operator/=(const Image_t&);

            // @brief access front element
            auto begin();
            auto begin() const;

            // @brief access past-the-end element
            auto end();
            auto end() const;

        protected:
            Eigen::Matrix<Value_t, Eigen::Dynamic, Eigen::Dynamic> _data;

        private:
            Value_t get_pixel_out_of_bounds(int x, int y) const;
            PaddingType _padding_type = PaddingType::STRETCH;

            struct Iterator
            {
                public:
                    Iterator(Image_t*, size_t x, size_t y);

                    using iterator_category = std::bidirectional_iterator_tag;
                    using value_type = Value_t;
                    using difference_type = int;
                    using pointer = Value_t*;
                    using reference = Value_t&;

                    bool operator==(Iterator& other) const;
                    bool operator!=(Iterator& other) const;

                    Iterator& operator++();
                    Iterator& operator--();

                    Value_t& operator*() const;
                    Iterator& operator=(Value_t);

                private:
                    Image_t* _data;
                    Vector2ui _size;
                    size_t _x, _y = 0;
            };

            struct ConstIterator
            {
                public:
                    ConstIterator(const Image_t*, size_t x, size_t y);

                    using iterator_category = std::bidirectional_iterator_tag;
                    using value_type = const Value_t;
                    using difference_type = int;
                    using pointer = const Value_t*;
                    using reference = const Value_t&;

                    bool operator==(ConstIterator& other) const;
                    bool operator!=(ConstIterator& other) const;

                    ConstIterator& operator++();
                    ConstIterator& operator--();

                    const Value_t& operator*() const;

                private:
                    const Image_t* _data;
                    Vector2ui _size;
                    size_t _x, _y = 0;
            };
    };
}

#include ".src/multi_plane_image.inl"
#include ".src/multi_plane_image_iterator.inl"