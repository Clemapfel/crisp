// 
// Copyright 2021 Clemens Cords
// Created on 10.09.21 by clem (mail@clemens-cords.com)
//

#include <gpu_side/texture.hpp>
#include <gpu_side/state.hpp>

namespace crisp
{
    void MorphologicalTransform::set_structuring_element(StructuringElement se)
    {
        _origin = Vector2ui{size_t(se.rows()) / 2, size_t(se.cols()) / 2};
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
                        for (int a = 0; a < n; ++a)
                        {
                            for (int b = 0; b < m; ++b)
                            {
                                if (not se(a, b).has_value())
                                    continue;

                                if (se(a, b) and not img_in(x + a - origin.x(), y + b - origin.y()).at(i))
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
                        for (int a = 0; a < n; ++a)
                        {
                            for (int b = 0; b < m ; ++b)
                            {
                                if (not se(a, b).has_value())
                                    continue;

                                if (se(a, b))
                                {
                                    min = std::min(img_in(x + a - origin.x(), y + b - origin.y()).at(i), min);
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
                        for (int a = 0; a < n; ++a)
                        {
                            for (int b = 0; b < m; ++b)
                            {
                                if (not se(a, b).has_value())
                                    continue;

                                if (se(a, b) and img_in(x + a - origin.x(), y + b - origin.x()).at(i))
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
                        for (int a = 0; a < n; ++a)
                        {
                            for (int b = 0; b < m; ++b)
                            {
                                if (not se(a, b).has_value())
                                    continue;

                                if (se(a, b))
                                {
                                    max = std::max(img_in(x + a - origin.x(), y + b - origin.x()).at(i), max);
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

    template<typename T, size_t N>
    void MorphologicalTransform::erode(Texture<T, N>& texture)
    {
        auto workspace = texture._workspace;
        
        assert(_structuring_element.rows() < 5 and _structuring_element.cols() < 5 && "GPU-side morphological transforms are only supported for structuring elements of size m*n where m, n in {2, 3, 4}");

        std::stringstream shader_id;
        shader_id << "erode_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        auto shader = State::register_shader(shader_id.str());
        auto program = State::register_program(shader);
        State::free_shader(shader);

        auto size = State::register_vec2(texture.get_size());
        auto se = State::register_structuring_element(_structuring_element);

        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();
        workspace.yield();

        State::free_program(program);
        State::free_vec2(size);
        State::free_matrix(se);
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

    template<typename T, size_t N>
    void MorphologicalTransform::dilate(Texture<T, N>& texture)
    {
        auto workspace = texture._workspace;

        assert(_structuring_element.rows() < 5 and _structuring_element.cols() < 5 && "GPU-side morphological transforms are only supported for structuring elements of size m*n where m, n in {2, 3, 4}");

        std::stringstream shader_id;
        shader_id << "dilate_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        auto shader = State::register_shader(shader_id.str());
        auto program = State::register_program(shader);
        State::free_shader(shader);

        auto size = State::register_vec2(texture.get_size());
        auto se = State::register_structuring_element(_structuring_element);

        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();
        workspace.yield();

        State::free_program(program);
        State::free_vec2(size);
        State::free_matrix(se);
    }

    template<typename Image_t>
    void MorphologicalTransform::erode(Image_t& image, const Image_t& mask)
    {
        assert(mask.get_size() == image.get_size());

        Image_t result;
        result.create(image.get_size().x(), image.get_size().y());

        erode_aux(image, result);

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().x(); ++y)
            {
                if (mask(x, y) <= image(x, y))
                    image(x, y) = std::max(result(x, y), mask(x, y));
            }
        }
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
            {
                if (image(x, y) == result(x, y))
                    continue;
                else
                    image(x, y) = std::min(result(x, y), mask(x, y));
            }
    }

    template<typename Image_t>
    void MorphologicalTransform::open(Image_t& image)
    {
        erode(image);
        dilate(image);
    }

    template<typename T, size_t N>
    void MorphologicalTransform::open(Texture<T, N>& texture)
    {
        auto workspace = texture._workspace;

        assert(_structuring_element.rows() < 5 and _structuring_element.cols() < 5 && "GPU-side morphological transforms are only supported for structuring elements of size m*n where m, n in {2, 3, 4}");

        std::stringstream shader_id;
        shader_id << "erode_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        auto shader = State::register_shader(shader_id.str());
        auto program = State::register_program(shader);
        State::free_shader(shader);

        auto size = State::register_vec2(texture.get_size());
        auto se = State::register_structuring_element(_structuring_element);

        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();

        State::free_program(program);
        std::stringstream new_shader_id;
        new_shader_id << "dilate_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        shader = State::register_shader(new_shader_id.str());
        program = State::register_program(shader);
        State::free_shader(shader);
        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();
        workspace.yield();

        State::free_program(program);
        State::free_vec2(size);
        State::free_matrix(se);
    }

    template<typename Image_t>
    void MorphologicalTransform::close(Image_t& image)
    {
        dilate(image);
        erode(image);
    }

    template<typename T, size_t N>
    void MorphologicalTransform::close(Texture<T, N>& texture)
    {
        auto workspace = texture._workspace;

        assert(_structuring_element.rows() < 5 and _structuring_element.cols() < 5 && "GPU-side morphological transforms are only supported for structuring elements of size m*n where m, n in {2, 3, 4}");

        std::stringstream shader_id;
        shader_id << "dilate_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        auto shader = State::register_shader(shader_id.str());
        auto program = State::register_program(shader);
        State::free_shader(shader);

        auto size = State::register_vec2(texture.get_size());
        auto se = State::register_structuring_element(_structuring_element);

        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();

        State::free_program(program);
        std::stringstream new_shader_id;
        new_shader_id << "erode_mat" << _structuring_element.cols() << "x" << _structuring_element.rows() << ".glsl";

        shader = State::register_shader(new_shader_id.str());
        program = State::register_program(shader);
        State::free_shader(shader);
        State::bind_shader_program(program);
        State::bind_texture(program, "_texture", texture.get_handle());
        State::bind_vec2(program, "_texture_size", size);
        State::bind_matrix(program, "_structuring_element", se);

        workspace.display();
        workspace.yield();

        State::free_program(program);
        State::free_vec2(size);
        State::free_matrix(se);
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

    StructuringElement MorphologicalTransform::square(long size)
    {
        return all_foreground(size, size);
    }
    
    StructuringElement MorphologicalTransform::cross(long size)
    {
        StructuringElement out = all_dont_care(size, size);
        
        for (size_t i = 0; i < size; ++i)
        {
            for (int offset = -(float(size)/3/2); offset <= (float(size)/3/2); offset++)
            {
                out((out.rows() - 1) / 2 + offset,  i) = true;
                out(i, (out.rows() - 1) / 2 + offset) = true;
            }
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

        out(size / 2, size / 2) = true;
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

    template<typename Image_t>
    void MorphologicalTransform::replace_pattern(Image_t& image, const StructuringElement& replacement)
    {
        assert(_structuring_element.rows() == replacement.rows() and _structuring_element.cols() == replacement.cols());

        long n = _structuring_element.rows(),
             m = _structuring_element.cols();

        auto origin = Vector2i{int(_origin.x()), int(_origin.y())};

        Image_t result = image;

        using ImageValue_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        for (long x = 0; x < image.get_size().x(); ++x)
        {
            for (long y = 0; y < image.get_size().y(); ++y)
            {
                for (size_t i = 0; i < ImageValue_t::size(); ++i)
                {
                    for (int a = -origin.x(); a < n - origin.x(); ++a)
                    {
                        for (int b = -origin.y(); b < m - origin.y(); ++b)
                        {
                            if (not _structuring_element(a + origin.x(), b + origin.y()).has_value())
                                continue;

                            if (image(x + a, y + b).at(i) != Inner_t(_structuring_element(a + origin.x(), b + origin.y()).value()))
                            {
                                goto next;
                            }
                        }
                    }

                    // found
                    for (int a = -origin.x(); a < n - origin.x(); ++a)
                    {
                        for (int b = -origin.y(); b < m - origin.y(); ++b)
                        {
                            if (not _structuring_element(a + origin.x(), b + origin.y()).has_value())
                                continue;

                            result(x + a, y + b).at(i) = replacement(a + origin.x(), b + origin.y()).value_or(false);
                        }
                    }

                    next:;
                }
            }
        }

        for (long i = 0; i < image.get_size().x(); ++i)
            for (long j = 0; j < image.get_size().y(); ++j)
                image(i, j) = result(i, j);
    }

    NonFlatStructuringElement MorphologicalTransform::square_pyramid(long dimensions)
    {
        NonFlatStructuringElement out;
        out.resize(dimensions, dimensions);
        out.setConstant(std::optional<float>());

        float max = 1, min = 0;
        const float step = (max - min) / ((dimensions - 1) / 2);
        float current_color = min;
        size_t offset = 0;

        while (offset < ((dimensions - 1) / 2))
        {
            for (long x = offset; x < dimensions - offset; ++x)
            {
                out(x, offset) = current_color;
                out(x, dimensions - offset - 1) = current_color;
            }

            for (long y = offset; y < dimensions - offset; ++y)
            {
                out(offset, y) = current_color;
                out(dimensions - offset - 1, y) = current_color;
            }

            current_color += step;
            offset += 1;
        }

        return out;
    }

    NonFlatStructuringElement MorphologicalTransform::diamond_pyramid(long dimensions)
    {
        NonFlatStructuringElement out;
        out.resize(dimensions, dimensions);
        out.setConstant(std::optional<float>());
        
        float max = 1, min = 0;
        
        const float step = (max - min) / ((dimensions - 1) / 2);
        float current_color = float(min);
        size_t offset = 0;

        long half = ((dimensions - 1) / 2);
        while (offset < half)
        {
            for (long x = half, y = offset; x < dimensions and y <= half; x++, y++)
            {
                out(x, y) = current_color;
                out(dimensions - x - 1, y) = current_color;
                out(x, dimensions - y - 1) = current_color;
                out(dimensions - x - 1, dimensions - y - 1) = current_color;
            }
            
            offset += 1;
            current_color += step;
        }

        return out;
    }

    NonFlatStructuringElement MorphologicalTransform::cone(long dimensions)
    {
        NonFlatStructuringElement out;
        out.resize(dimensions, dimensions);
        out.setConstant(std::optional<float>());

        float max = 1, min = 0;

        assert(dimensions % 2 == 1 && "dimensions have to be odd for the structuring element to be rotationally symmetrical");

        long radius = (dimensions - 1) / 2;
        auto dist = [dimensions, r = radius](long x, long y) -> float {return sqrt((x-r)*(x-r) + (y-r)*(y-r));};

        const float step = (max - min) / ((dimensions - 1) / 2);
        float current_color = float(min);
        size_t offset = 0;

        for (long i = 0; i < dimensions; ++i)
            for (long j = 0; j < dimensions; ++j)
                if (dist(i, j) <= radius)
                    out(i, j) = (1 - (dist(i, j) / radius)) * (max - min) + min;
                else
                    out(i, j) = 0; //std::optional<float>();

        return out;
    }

    NonFlatStructuringElement MorphologicalTransform::hemisphere(long dimensions)
    {
        NonFlatStructuringElement out;
        out.resize(dimensions, dimensions);
        out.setConstant(std::optional<float>());

        float max = 1, min = 0;

        assert(dimensions % 2 == 1 && "dimensions have to be odd for the structuring element to be rotationally symmetrical");

        long radius = (dimensions - 1) / 2;
        auto dist = [dimensions, r = radius](long x, long y) -> float {return sqrt((x-r)*(x-r) + (y-r)*(y-r));};

        const float step = (max - min) / ((dimensions - 1) / 2);
        float current_color = float(min);
        size_t offset = 0;

        for (long i = 0; i < dimensions; ++i)
        {
            for (long j = 0; j < dimensions; ++j)
            {
                if (dist(i, j) <= radius)
                {
                    float v = dist(i, j) / radius;
                    out(i, j) = sqrt(1 - v * v);
                }
                else
                    out(i, j) = std::optional<float>();
            }
        }

        return out;
    }



}