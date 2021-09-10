// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#include <morphological_transform.hpp>

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

    bool& MorphologicalTransform::operator()(size_t x, size_t y)
    {
        return _structuring_element(x, y);
    }

    bool MorphologicalTransform::operator()(size_t x, size_t y) const
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
                ImageValue_t out;

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
                        else
                            out.at(i) = true;
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

    StructuringElement MorphologicalTransform::all_foreground(long nrows, long ncols)
    {
        StructuringElement out;
        out.resize(nrows, ncols);

        out.setConstant(true);

        return out;
    }

}