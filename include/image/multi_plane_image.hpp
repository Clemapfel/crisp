// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector.hpp>
#include <image/padding_type.hpp>
#include <color.hpp>

#include <type_traits>

namespace crisp
{
    /// @brief an image that lives in ram and is operated upon by the cpu
    template<typename InnerValue_t, size_t N = 1>
    class Image
    {
        public:
            /// @brief non-const iterator, iterates left-to-right, top-to-bottom
            class Iterator;

            /// @brief const iterator, iterates left-to-right, top-to-bottom
            class ConstIterator;

            /// @brief expose pixel value type
            using Value_t = typename std::conditional<N == 3 and std::is_same_v<InnerValue_t, float>, RGB, Vector<InnerValue_t, N>>::type;

            /// @brief number of pixel value type components
            static constexpr size_t n_planes = N;

            /// @brief default ctor
            Image() = default;

            /// @brief create image of specified size and value
            /// @param width: x-dimension of the image
            /// @param height: y-dimension of the image
            /// @param init: initial value
            Image(size_t width, size_t height, Value_t init = Value_t());

            /// @brief create image of specified size and value
            /// @param width: x-dimension of the image
            /// @param height: y-dimension of the image
            /// @param init: initial value
            void create(size_t width, size_t height, Value_t init = Value_t());

            /// @brief access pixel or padding if out of range
            /// @param x: row index
            /// @param y: column index
            /// @returns const reference to value
            virtual const Value_t& operator()(int x, int y) const;

            /// @brief access pixel or padding if out of range
            /// @param x: row index
            /// @param y: column index
            /// @returns const reference to value, if the index is out of bounds, modifying the reference has no effect on the image
            virtual Value_t& operator()(int x, int y);

            /// @brief access pixel with bounds checking
            /// @param x: row index
            /// @param y: column index
            /// @returns const reference to value
            virtual const Value_t& at(size_t x, size_t y) const;

            /// @brief access pixel with bounds checking
            /// @param x: row index
            /// @param y: column index
            /// @returns reference to value
            virtual Value_t& at(size_t x, size_t y);

            /// @brief get number of pixels
            /// @returns vector where .x is the width, .y the height
            Vector2ui get_size() const;

            /// @brief specify the padding type, STRETCH by default
            /// @param padding_type
            void set_padding_type(PaddingType);

            /// @brief access the padding type
            /// @returns padding type
            PaddingType get_padding_type() const;

            /// @brief image-image arithmetic elementwise addition
            /// @param other_image
            /// @returns resulting image
            Image<InnerValue_t, N> operator+(const Image<InnerValue_t, N>&) const;

            /// @brief image-image arithmetic elementwise subtraction
            /// @param other_image
            /// @returns resulting image
            Image<InnerValue_t, N> operator-(const Image<InnerValue_t, N>&) const;

            /// @brief image-image arithmetic elementwise multiplication
            /// @param other_image
            /// @returns resulting image
            Image<InnerValue_t, N> operator*(const Image<InnerValue_t, N>&) const;

            /// @brief image-image arithmetic elementwise addition
            /// @param other_image
            /// @returns resulting image
            Image<InnerValue_t, N> operator/(const Image<InnerValue_t, N>&) const;

            /// @brief image-image arithmetic elementwise addition assignment
            /// @param other_image
            /// @returns reference to self
            Image<InnerValue_t, N>& operator+=(const Image<InnerValue_t, N>&);

            /// @brief image-image arithmetic elementwise subtraction assignment
            /// @param other_image
            /// @returns reference to self
            Image<InnerValue_t, N>& operator-=(const Image<InnerValue_t, N>&);

            /// @brief image-image arithmetic elementwise multiplication assignment
            /// @param other_image
            /// @returns reference to self
            Image<InnerValue_t, N>& operator*=(const Image<InnerValue_t, N>&);

            /// @brief image-image arithmetic elementwise division assignment
            /// @param other_image
            /// @returns reference to self
            Image<InnerValue_t, N>& operator/=(const Image<InnerValue_t, N>&);
            
            /// @brief access all pixels nth component as a picture
            /// @returns new 1d image holding copied values of the component
            template<size_t PlaneIndex>
            Image<InnerValue_t, 1> get_nths_plane() const;

            /// @brief set the nths component of each pixel
            /// @param plane: image holding the new values
            template<size_t PlaneIndex>
            void set_nths_plane(const Image<InnerValue_t, 1>&);

            /// @brief non-const begin
            /// @return non-const iterator to top-left element
            auto begin();

            /// @brief const begin
            /// @return const iterator to top-left element
            auto begin() const;

            /// @brief non-const end
            /// @return non-const iterator to past-the-end element
            auto end();

            /// @brief const begin
            /// @return const iterator to past-the-end element
            auto end() const;

        protected:
            Eigen::Matrix<Value_t, Eigen::Dynamic, Eigen::Dynamic> _data;

        private:
            Value_t get_pixel_out_of_bounds(int x, int y) const;
            mutable Value_t _dummy_padding_reference;

            PaddingType _padding_type = PaddingType::STRETCH;

            struct Iterator
            {
                public:
                    Iterator(Image<InnerValue_t, N>*, size_t x, size_t y);

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
                    Image<InnerValue_t, N>* _data;
                    Vector2ui _size;
                    size_t _x, _y = 0;
            };

            struct ConstIterator
            {
                public:
                    ConstIterator(const Image<InnerValue_t, N>*, size_t x, size_t y);

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
                    const Image<InnerValue_t, N>* _data;
                    Vector2ui _size;
                    size_t _x, _y = 0;
            };
    };
}

#include ".src/multi_plane_image.inl"
#include ".src/multi_plane_image_iterator.inl"