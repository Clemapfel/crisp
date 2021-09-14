// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    void MorphologicalTransform::set_structuring_element(StructuringElement se)
    {
        _structuring_element = std::move(se);
    }

    StructuringElement& MorphologicalTransform::get_structuring_element()
    {
        return _structuring_element;
    }

    std::optional<bool>& MorphologicalTransform::operator()(size_t x, size_t y)
    {
        return _structuring_element(x, y);
    }

    std::optional<bool> MorphologicalTransform::operator()(size_t x, size_t y) const
    {
        return _structuring_element(x, y);
    }

    void MorphologicalTransform::set_structuring_element_origin(size_t x, size_t y)
    {
        _origin = Vector2ui{x, y};
    }

    Vector2ui MorphologicalTransform::get_structuring_element_origin() const
    {
        return _origin;
    }

    template<typename Image_t>
    void MorphologicalTransform::erode_aux(Image_t& img_in, Image_t& img_out)
    {
        const auto& se = _structuring_element;

        int n = se.rows(),
            m = se.cols();

        auto origin = Vector2i{int(_origin.x()), int(_origin.y())};

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        for (int y = 0; y < img_in.get_size().y(); ++y)
        {
            for (int x = 0; x < img_in.get_size().x(); ++x)
            {
                ImageValue_t out = img_in(x, y);

                // binary
                if (std::is_same_v<Inner_t, bool>)
                {
                    for (size_t i = 0; i < ImageValue_t::size(); ++i)
                    {
                        bool missed = false;
                        for (int a = -origin.x(); a < n -origin.x(); ++a)
                        {
                            for (int b = -origin.y(); b < m -origin.y(); ++b)
                            {
                                if (not se(a + origin.x(), b + origin.y()).has_value())
                                    continue;

                                if (se(a + origin.x(), b + origin.y()) and not img_in(x + a, y + b).at(i))
                                {
                                    missed = true;
                                    goto skip;
                                }
                            }
                        }

                        skip:
                        if (missed)
                            out.at(i) = false;
                    }
                }
                else
                {
                    for (size_t i = 0; i < ImageValue_t::size(); ++i)
                    {
                        auto min = img_in(x, y).at(i);
                        for (int a = -origin.x(); a < n - origin.x(); ++a)
                        {
                            for (int b = -origin.y(); b < m - origin.y(); ++b)
                            {
                                if (not se(a + origin.x(), b + origin.y()).has_value())
                                    continue;

                                if (se(a + origin.x(), b + origin.y()))
                                {
                                    min = std::min(img_in(x + a, y + b).at(i), min);
                                }
                            }
                        }

                        out.at(i) = min;
                    }
                }

                img_out(x, y) = out;
            }
        }
    }

    template<typename Image_t>
    void MorphologicalTransform::dilate_aux(Image_t& img_in, Image_t& img_out)
    {
        const auto& se = _structuring_element;

        int n = se.rows(),
            m = se.cols();

        auto origin = Vector2i{int(_origin.x()), int(_origin.y())};

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        for (int y = 0; y < img_in.get_size().y(); ++y)
        {
            for (int x = 0; x < img_in.get_size().x(); ++x)
            {
                ImageValue_t out;

                // binary
                if (std::is_same_v<Inner_t, bool>)
                {
                    for (size_t i = 0; i < ImageValue_t::size(); ++i)
                    {
                        bool found = false;
                        for (int a = -origin.x(); a < n -origin.x(); ++a)
                        {
                            for (int b = -origin.y(); b < m -origin.y(); ++b)
                            {
                                if (not se(a + origin.x(), b + origin.y()).has_value())
                                    continue;

                                if (se(a + origin.x(), b + origin.y()) and img_in(x + a, y + b).at(i))
                                {
                                    found = true;
                                    goto skip;
                                }
                            }
                        }

                        skip:
                        if (found)
                            out.at(i) = true;
                    }
                }
                else
                {
                    for (size_t i = 0; i < ImageValue_t::size(); ++i)
                    {
                        auto max = img_in(x, y).at(i);
                        for (int a = -origin.x(); a < n - origin.x(); ++a)
                        {
                            for (int b = -origin.y(); b < m - origin.y(); ++b)
                            {
                                if (not se(a + origin.x(), b + origin.y()).has_value())
                                    continue;

                                if (se(a + origin.x(), b + origin.y()))
                                {
                                    max = std::max(img_in(x + a, y + b).at(i), max);
                                }
                            }
                        }

                        out.at(i) = max;
                    }
                }

                img_out(x, y) = out;
            }
        }
    }

    template<typename Image_t>
    void MorphologicalTransform::erode(Image_t& image)
    {
        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        erode_aux(image, result);

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().x(); ++y)
                image(x, y) = result(x, y);
    }

    template<typename Image_t>
    void MorphologicalTransform::dilate(Image_t& image)
    {
        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        dilate_aux(image, result);

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().x(); ++y)
                image(x, y) = result(x, y);
    }

    template<typename Image_t>
    void MorphologicalTransform::erode(Image_t& image, const Image_t& mask)
    {
        assert(mask.get_size() == image.get_size());

        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        erode_aux(image, result);

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().x(); ++y)
                image(x, y) = std::max(result(x, y), mask(x, y));
    }

    template<typename Image_t>
    void MorphologicalTransform::dilate(Image_t& image, const Image_t& mask)
    {
        assert(mask.get_size() == image.get_size());

        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        dilate_aux(image, result);

        for (long x = 0; x < image.get_size().x(); ++x)
            for (long y = 0; y < image.get_size().x(); ++y)
                image(x, y) = std::min(result(x, y), mask(x, y));
    }

    template<typename Image_t>
    void MorphologicalTransform::open(Image_t& image)
    {
        erode(image);
        dilate(image);
    }

    template<typename Image_t>
    void MorphologicalTransform::close(Image_t& image)
    {
        dilate(image);
        erode(image);
    }

    StructuringElement MorphologicalTransform::all_dont_care(long nrows, long ncols)
    {
        StructuringElement out;
        out.resize(nrows, ncols);

        out.setConstant(std::optional<bool>());

        return out;
    }

    StructuringElement MorphologicalTransform::all_foreground(long nrows, long ncols)
    {
        StructuringElement out;
        out.resize(nrows, ncols);

        out.setConstant(true);

        return out;
    }
    
    StructuringElement MorphologicalTransform::all_background(long nrows, long ncols)
    {
        StructuringElement out;
        out.resize(nrows, ncols);

        out.setConstant(false);

        return out;
    }
    
    StructuringElement MorphologicalTransform::cross(long size)
    {
        assert(size % 2 == 1 && "dimensions have to be odd for the structuring element to be rotationally symmetrical");

        StructuringElement out = all_dont_care(size, size);
        
        for (size_t i = 0; i < size; ++i)
        {
            out((out.rows() - 1) / 2, i) = true;
            out(i, (out.rows() - 1) / 2) = true;
        }

        return out;
    }
    
    StructuringElement MorphologicalTransform::diamond(long size)
    {
        // has to be odd to be rotationally symmetrical
        if (size % 2 == 0)
            size += 1;

        StructuringElement out = all_dont_care(size, size);
        
        long half = ((size - 1) / 2);
        size_t offset = 0;
        while (offset < half)
        {
            for (long x = half, y = offset; x < size and y <= half; x++, y++)
            {
                out(x, y) = true;
                out(size - x - 1, y) = true;
                out(x, size - y - 1) = true;
                out(size - x - 1, size - y - 1) = true;
            }

            offset += 1;
        }

        return out;
    }

    StructuringElement MorphologicalTransform::circle(long size)
    {
        if (size % 2 == 0)
            size += 1;

        StructuringElement out = all_dont_care(size, size);

        long radius = (size - 1) / 2;

        auto mark = [size, r = radius](long x, long y) -> bool {return (sqrt((x-r)*(x-r) + (y-r)*(y-r))) < r;};

        for (long x = 0; x < size; ++x)
            for (long y = 0; y < size; ++y)
                if (mark(x, y))
                    out(x, y) = true;

        return out;
    }

    template<typename Image_t>
    void MorphologicalTransform::hit_or_miss_transform(Image_t& image)
    {
        long n = _structuring_element.rows(),
             m = _structuring_element.cols();

        auto origin = Vector2i{int(_origin.x()), int(_origin.y())};

        Image_t result;

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        result.create(image.get_size().x(), image.get_size().y(), ImageValue_t(Inner_t(0)));

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                for (size_t i = 0; i < ImageValue_t::size(); ++i)
                {
                    bool found = true;
                    for (int a = -origin.x(); a < n - origin.x(); ++a)
                    {
                        for (int b = -origin.y(); b < m - origin.y(); ++b)
                        {
                            if (not _structuring_element(a + origin.x(), b + origin.y()).has_value())
                                continue;

                            if (image(x + a, y + b).at(i) != Inner_t(_structuring_element(a + origin.x(), b + origin.y()).value()))
                            {
                                found = false;
                                goto no_hit;
                            }
                        }
                    }

                    no_hit:
                    result(x, y).at(i) = Inner_t(found);
                }
            }
        }

        for (long i = 0; i < image.get_size().x(); ++i)
            for (long j = 0; j < image.get_size().y(); ++j)
                image(i, j) = result(i, j);
    }
}