// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#include <image_region.hpp>
#include <Eigenvalues>

namespace crisp
{
    template<typename Image_t>
    ImageRegion<Image_t>::ImageRegion(const ImageSegment& segment, const Image_t& image)
    {
        create_from(segment, image);
    }

    template<typename Image_t>
    ImageRegion<Image_t>::ImageRegion(const Image_t& image)
    {
        auto segment = ImageSegment();
        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
                segment.insert(Vector2ui{x, y});

        create_from(segment, image);
    }

    template<typename Image_t>
    void ImageRegion<Image_t>::create_from(const ImageSegment& segment, const Image_t& image)
    {
        // init
        _elements.clear();

        unsigned int min_x = image.get_size().x(),
                max_x = 0;
        unsigned int min_y = image.get_size().y(),
                max_y = 0;

        ImageSegment strong_pixels;
        ImageSegment weak_pixels;

        std::map<size_t, size_t> hash_to_n_occurences;
        size_t max_n_occurence = 0;

        for (auto& px : segment)
        {
            _elements.insert(Element{px, image(px.x(), px.y())});

            size_t n_unconnected = 0;
            for (int i = -1; i <= +1; ++i)
            {
                for (int j = -1; j <= +1; ++j)
                {
                    // outer edge of image is always boundary
                    if (px.x() + i < 0 or px.x() + i > image.get_size().x() or px.y() + j < 0 or
                        px.y() + j > image.get_size().y())
                        n_unconnected++;

                    else if (not (i == 0 and j == 0) and
                             segment.find(Vector2ui{px.x() + i, px.y() + j}) == segment.end())
                        n_unconnected++;

                    auto hash = px.to_hash();
                    if (hash_to_n_occurences.find(hash) == hash_to_n_occurences.end())
                        hash_to_n_occurences.emplace(hash, 0);

                    hash_to_n_occurences.at(hash) += 1;
                    max_n_occurence = std::max(max_n_occurence, hash_to_n_occurences.at(hash));

                    min_x = std::min<unsigned int>(min_x, px.x());
                    max_x = std::max<unsigned int>(max_x, px.x());
                    min_y = std::min<unsigned int>(min_y, px.y());
                    max_y = std::max<unsigned int>(max_y, px.y());
                }
            }

            if (n_unconnected > 1)
                strong_pixels.insert(px);
            else if (n_unconnected == 1)
                weak_pixels.insert(px);
        }

        _max_probability = float(max_n_occurence) / float(_elements.size());

        _x_bounds = {min_x, max_x};
        _y_bounds = {min_y, max_y};

        // trace boundary
        _boundary.clear();

        auto translate_in_direction = [&](Vector2ui c, uint8_t direction) -> Vector2ui {
            direction = direction % 8;
            int x_offset, y_offset;
            switch (direction)
            {
                case 0: // WEST
                    x_offset = -1;
                    y_offset = 0;
                    break;

                case 1: // SOUTH WEST
                    x_offset = -1;
                    y_offset = +1;
                    break;

                case 2: // SOUTH
                    x_offset = 0;
                    y_offset = +1;
                    break;

                case 3: // SOUTH EAST
                    x_offset = +1;
                    y_offset = +1;
                    break;

                case 4: // EAST
                    x_offset = +1;
                    y_offset = 0;
                    break;

                case 5: // NORTH EAST
                    x_offset = +1;
                    y_offset = -1;
                    break;

                case 6: // NORTH
                    x_offset = 0;
                    y_offset = -1;
                    break;

                case 7: // NORTH_WEST
                    x_offset = -1;
                    y_offset = -1;
                    break;

                default:
                    assert(false);
            }

            return Vector2ui{c.x() + x_offset, c.y() + y_offset};
        };

        std::vector<std::vector<Vector2ui>> boundaries_out;
        std::vector<std::vector<uint8_t>> directions_out;

        while (not strong_pixels.empty())
        {
            boundaries_out.emplace_back();
            directions_out.emplace_back();

            auto& boundary = boundaries_out.back();
            auto& direction = directions_out.back();

            auto top_left = *strong_pixels.begin();
            boundary.push_back(top_left);
            strong_pixels.erase(top_left);
            direction.push_back(0);

            size_t current_i = 0;
            bool finished_maybe = false;
            do
            {
                auto current = boundary.at(current_i);
                auto current_direction = direction.at(current_i);

                bool found = false;

                // check strong candidates
                for (int dir = current_direction - 1, n = 0; n < 8; ++dir, ++n)
                {
                    auto to_check = translate_in_direction(current, dir);

                    if (to_check.x() < _x_bounds.x() or to_check.x() > _x_bounds.y() or
                        to_check.y() < _y_bounds.x() or to_check.y() > _y_bounds.y())
                        continue;

                    if (to_check == top_left)
                        finished_maybe = true;

                    if (strong_pixels.find(to_check) != strong_pixels.end())
                    {
                        boundary.push_back(to_check);
                        direction.push_back(dir);
                        strong_pixels.erase(to_check);
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    current_i = boundary.size() - 1;
                    continue;
                }
                else if (finished_maybe)
                    break;

                // check weak candidates
                for (int dir = current_direction - 1, n = 0; n < 8; ++dir, ++n)
                {
                    auto to_check = translate_in_direction(current, dir);

                    if (to_check.x() < _x_bounds.x() or to_check.x() > _x_bounds.y() or
                        to_check.y() < _y_bounds.x() or to_check.y() > _y_bounds.y())
                        continue;

                    if (weak_pixels.find(to_check) != weak_pixels.end())
                    {
                        boundary.push_back(to_check);
                        direction.push_back(dir);
                        weak_pixels.erase(to_check);
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    current_i = boundary.size() - 1;
                    continue;
                }
                else
                {
                    if (current_i == 0)
                        break;
                    else
                        current_i--;
                }
            } while (true);
        }

        _boundary = boundaries_out.at(0);
        _boundary_direction = directions_out.at(0);

        _n_holes = boundaries_out.size() - 1;

        _hole_boundaries.clear();
        for (size_t i = 1; i < boundaries_out.size(); ++i)
            _hole_boundaries.push_back(boundaries_out.at(i));

        auto turn_type = [&](size_t i_a, size_t i_b) -> int
        {
            auto point_a = _boundary.at(i_a),
                 point_b = _boundary.at(i_b);

            // warp point
            if (abs(int(point_a.x()) - int(point_b.x())) > 1 or
                abs(int(point_a.y()) - int(point_b.y())) > 1)
                return 0;

            auto dir_a = _boundary_direction.at(i_a),
                 dir_b = _boundary_direction.at(i_b);

            if (dir_b > dir_a or (dir_a == 7 and dir_b == 0))
                return -1; // left-hand turn
            else if (dir_b < dir_a or (dir_a == 0 and dir_b == 7))
                return +1; // right-hand turn
            else
                return 0; // colinear
        };

        _boundary_polygon.clear();

        Vector2f mean_pos = Vector2f{0, 0};

        for (size_t i = 0; i < _boundary.size() - 1; ++i)
        {
            mean_pos += Vector2f{float(_boundary.at(i).x()), float(_boundary.at(i).y())};

            if (turn_type(i, i+1) != 0)
                _boundary_polygon.push_back(_boundary.at(i));
        }

        _centroid = mean_pos / float(_boundary.size());

        // compute minor and major axis
        float mean_x = 0, mean_y = 0;

        for (auto& px : _boundary)
        {
            mean_x += px.x();
            mean_y += px.y();
        }

        mean_x /= _boundary.size();
        mean_y /= _boundary.size();

        Eigen::Matrix<float, 2, 2> covar;

        for (auto& px : _boundary)
        {
            Eigen::Matrix<float, 2, 1> current;
            current << (px.x() - mean_x),
                       (px.y() - mean_y);

            auto current_covar = (current * current.transpose());
            covar += current_covar;
        }

        covar /= _elements.size();

        auto eigens = Eigen::EigenSolver<decltype(covar)>(covar);

        auto e1 = Eigen::Vector2f(eigens.eigenvectors()(0, 0).real(), eigens.eigenvectors()(0, 1).real());
        e1.normalize();
        auto e2 = Eigen::Vector2f(eigens.eigenvectors()(1, 0).real(), eigens.eigenvectors()(1, 1).real());
        e2.normalize();

        auto l1 = eigens.eigenvalues()( 0).real();
        auto l2 = eigens.eigenvalues()( 1).real();

        auto centroid= Eigen::Vector2f(_centroid.x(), _centroid.y());

        auto to_crisp_vec = [](auto eigen)
        {
            return Vector2f{eigen(0), eigen(1)};
        };

        if (l1 > l2)
        {
            _major_axis.first = to_crisp_vec(centroid - l1 * e1);
            _major_axis.second = to_crisp_vec(centroid + l1 * e1);

            _minor_axis.first = to_crisp_vec(centroid - l2 * e2);
            _minor_axis.second = to_crisp_vec(centroid + l2 * e2);

            _eccentricity = sqrt(1 - (l1 / l2) * (l1 / l2));
        }
        else
        {
            _major_axis.first = to_crisp_vec(centroid - l2 * e2);
            _major_axis.second = to_crisp_vec(centroid + l2 * e2);

            _minor_axis.first = to_crisp_vec(centroid - l1 * e1);
            _minor_axis.second = to_crisp_vec(centroid + l1 * e1);

            _eccentricity = sqrt(1 - (l2 / l1) * (l2 / l1));
        }

        _co_occurence_matrices.clear();
    }

    template<typename Image_t>
    std::vector<float> ImageRegion<Image_t>::farthest_point_signature() const
    {
        // El-ghazal, A., Basir, O., & Belkasim, S. (2009). Farthest point distance: A new shape signature for Fourier descriptors. Signal Processing: Image Communication, 24(7), 572–586. doi:10.1016/j.image.2009.04.00
        auto value = [&](Vector2ui a)
        {
            // explicit casts for readability
            int a_x = a.x();
            int a_y = a.y();
            int c_x = _centroid.x();
            int c_y = _centroid.y();
            float a_to_centroid = sqrt((a_x - c_x) * (a_x - c_x) + (a_y - c_y) * (a_y - c_y));

            Vector2ui farthest_point = a;
            float max_dist = 0;
            for (const auto& point : _boundary)
            {
                int p_x = point.x();
                int p_y = point.y();
                float dist = sqrt((a_x - p_x) * (a_x - p_x) + (a_y - p_y) * (a_y - p_y));
                if (dist > max_dist)
                {
                    farthest_point = point;
                    max_dist = max_dist;
                }
            }

            int fp_x = farthest_point.x();
            int fp_y = farthest_point.y();
            float fp_to_centroid = sqrt((fp_x - c_x) * (fp_x - c_x) + (fp_y - c_y) * (fp_y - c_y));

            return a_to_centroid + fp_to_centroid;
        };

        std::vector<float> signature;
        signature.reserve(_boundary.size());

        for (auto& px : _boundary)
            signature.push_back(value(px));

        return signature;
    }

    template<typename Image_t>
    std::vector<float> ImageRegion<Image_t>::radial_distance_signature() const
    {
        auto value = [&](Vector2ui a)
        {
            int a_x = a.x();
            int a_y = a.y();
            int c_x = _centroid.x();
            int c_y = _centroid.y();
            return sqrt((a_x - c_x) * (a_x - c_x) + (a_y - c_y) * (a_y - c_y));
        };

        std::vector<float> signature;
        signature.reserve(_boundary.size());

        for (auto& px : _boundary)
            signature.push_back(value(px));

        return signature;
    }

    template<typename Image_t>
    std::vector<std::complex<float>> ImageRegion<Image_t>::complex_coordinate_signature() const
    {
        std::vector<std::complex<float>> signature;
        signature.reserve(_boundary.size());

        for (auto& px : _boundary)
            signature.emplace_back(px.x() - _centroid.x(), px.y() - _centroid.y());

        return signature;
    }

    template<typename Image_t>
    std::vector<float> ImageRegion<Image_t>::slope_chain_code_signature() const
    {
        std::vector<float> signature;
        for (size_t i = 0; i < _boundary_polygon.size() - 2; ++i)
        {
            auto a = _boundary_polygon.at(i);
            auto b = _boundary_polygon.at(i + 1);
            signature.push_back(atan2(b.y() - a.y(), (b.x() - a.x())));
        }

        return signature;
    }

    template<typename Image_t>
    Vector2f ImageRegion<Image_t>::get_centroid() const
    {
        return _centroid;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_perimeter() const
    {
        return _boundary.size();
    }

    template<typename Image_t>
    std::array<Vector2ui, 4> ImageRegion<Image_t>::get_axis_aligned_bounding_box() const
    {
        auto out = std::array<Vector2ui, 4>();
        auto min_x = _x_bounds.x();
        auto max_x = _x_bounds.y();
        auto min_y = _y_bounds.x();
        auto max_y = _y_bounds.y();

        out.at(0) = {min_x, min_y};
        out.at(1) = {max_x, min_y};
        out.at(2) = {max_x, max_y};
        out.at(3) = {min_x, max_y};

        return out;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_area() const
    {
        return _elements.size();
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::begin() const
    {
        return _elements.begin();
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::begin()
    {
        return _elements.begin();
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::end()
    {
        return _elements.end();
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::end() const
    {
        return _elements.end();
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_compactness() const
    {
        return (_boundary.size() * _boundary.size()) / float(_elements.size());
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_circularity() const
    {
        return 4*M_PI*_elements.size() / float(_boundary.size() * _boundary.size());
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_nths_moment_invariant(size_t i)
    {
        // page 858, 4th edition Image Processing (gonzales, woods)
        auto normalized_central_moment = [&](size_t p, size_t q)
        {
            float moment = 0,
                  value_sum = 0;
            for (auto& element : _elements)
            {
                value_sum +=
                        pow(element.position.x() - _centroid.x(), p) * pow(element.position.y() - _centroid.y(), q) *
                        element.value;
                value_sum += element.value;
            }

            return moment / std::pow<float>(value_sum, (p+q) / 2.f + 1);
        };

        assert(i != 0 and i <= 7 && "only moments for n = {1, 2, 3, 4, 5, 6, 7} are supported");

        if (i == 1)
        {
            float m_20 = normalized_central_moment(2, 0);
            float m_02 = normalized_central_moment(0, 2);

            return m_20 + m_02;
        }
        else if (i == 2)
        {
            float m_20 = normalized_central_moment(2, 0);
            float m_02 = normalized_central_moment(0, 2);
            float m_11 = normalized_central_moment(1, 1);

            return pow(m_20 - m_02, 2) + 4 * (pow(m_11, 2));
        }
        else if (i == 3)
        {
            float m_30 = normalized_central_moment(3, 0);
            float m_03 = normalized_central_moment(0, 3);
            float m_12 = normalized_central_moment(1, 2);
            float m_21 = normalized_central_moment(2, 1);

            return pow(m_30 - 3*m_12, 2) + pow(3*m_21 - m_03, 2);
        }
        else if (i == 4)
        {
            float m_30 = normalized_central_moment(3, 0);
            float m_03 = normalized_central_moment(0, 3);
            float m_12 = normalized_central_moment(1, 2);
            float m_21 = normalized_central_moment(2, 1);

            return pow(m_30 + m_12, 2) + pow(m_21 + m_03, 2);
        }
        else if (i == 5)
        {
            float m_30 = normalized_central_moment(3, 0);
            float m_03 = normalized_central_moment(0, 3);
            float m_12 = normalized_central_moment(1, 2);
            float m_21 = normalized_central_moment(2, 1);

            return (m_30 - 3 * m_12) * (m_30 + m_12) * (pow(m_30 + m_12, 2) - 3 * pow(m_21 + m_03, 2)) +
                    (3 * m_21 - m_03) * (m_21 + m_03) * (3 * pow(m_30 + m_12, 2) - pow(m_21 + m_03, 2));
        }
        else if (i == 6)
        {
            float m_20 = normalized_central_moment(2, 0);
            float m_02 = normalized_central_moment(0, 2);
            float m_30 = normalized_central_moment(3, 0);
            float m_03 = normalized_central_moment(0, 3);
            float m_12 = normalized_central_moment(1, 2);
            float m_21 = normalized_central_moment(2, 1);
            float m_11 = normalized_central_moment(1, 1);

            return (m_20 - m_02) * (pow(m_30 + m_12, 2) - (pow(m_21 + m_03, 2))) + 4 * m_11 * (m_30 + m_12) * (m_21 + m_03);
        }
        else if (i == 7)
        {
            float m_20 = normalized_central_moment(2, 0);
            float m_02 = normalized_central_moment(0, 2);
            float m_30 = normalized_central_moment(3, 0);
            float m_03 = normalized_central_moment(0, 3);
            float m_12 = normalized_central_moment(1, 2);
            float m_21 = normalized_central_moment(2, 1);

            return (3 * m_21 - m_03) * (m_30 + m_12) * (pow(m_30 + m_12, 2) - 3 * pow(m_21 + m_03, 2)) +
                    (3 * m_12 - m_30) * (m_21 + m_03) * (3 * pow(m_30 + m_12, 2) - pow(m_21 + m_03, 2));
        }
    }

    template<typename Image_t>
    const std::vector<Vector2ui>& ImageRegion<Image_t>::get_boundary() const
    {
        return _boundary;
    }

    template<typename Image_t>
    const std::vector<Vector2ui>& ImageRegion<Image_t>::get_boundary_polygon() const
    {
        return _boundary_polygon;
    }

    template<typename Image_t>
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>&
    ImageRegion<Image_t>::get_covariance_matrix() const
    {
    }

    template<typename Image_t>
    const std::pair<Vector2f, Vector2f> & ImageRegion<Image_t>::get_major_axis() const
    {
        return _major_axis;
    }

    template<typename Image_t>
    const std::pair<Vector2f, Vector2f> & ImageRegion<Image_t>::get_minor_axis() const
    {
        return _minor_axis;
    }

    template<typename Image_t>
    const Eigen::Matrix<size_t, 256, 256>& ImageRegion<Image_t>::get_co_occurence_matrix(CoOccurenceDirection direction) const
    {
        assert(false && "DEBUG");

        using Value_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        Eigen::Matrix<size_t, 256, 256> out;

        auto process = [&](size_t x, size_t y, size_t x_2, size_t y_2)
        {
            auto& current = out.back();

            const Element* first = nulltpr;
            const Element* second = nulltpr;

            for (const auto& element : _elements)
            {
                if (element.position == Vector2ui{x, y})
                    first = &element;
                else if (element.position = Vector2ui{x_2, y_2})
                    second = &element;

                if (first != nullptr and second != nullptr)
                    break;
            }

            if (first == nullptr or second == nullptr)
                return;

            float first_value = 0, second_value = 0;
            for (size_t i = 0; i < Value_t::size(); ++i)
            {
                first_value += first->value.at(i);
                second_value += second->value.at(i);
            }

            first_value = (first_value / Value_t::size()) * 255;
            second_value = (second_value / Value_t::size()) * 255;

            out(size_t(first_value), size_t(second_value)) += 1;
        };


        for (size_t i = 0; i < Value_t::size(); ++i)
        {
            for (auto& element : _elements)
            {
                size_t x = element.position.x();
                size_t y = element.position.y();

                switch (direction)
                {
                    case PLUS_MINUS_ZERO:
                        process(x, y, x, y - 1);
                        break;
                    case PLUS_45:
                        process(x, y, x+1, y-1);
                        break;
                    case PLUS_90:
                        process(x, y, x+1, y);
                        break;
                    case PLUS_125:
                        process(x, y, x+1, y+1);
                        break;
                    case PLUS_MINUS_180:
                        process(x, y, x, y+1);
                        break;
                    case MINUS_125:
                        process(x, y, x-1, y+1);
                        break;
                    case MINUS_90:
                        process(x, y, x-1, y);
                        break;
                    case MINUS_45:
                        process(x, y, x-1, y-1);
                        break;
                }
            }
        }

        _co_occurence_matrices.insert(std::make_pair(direction, out));
        return _co_occurence_matrices.at(direction);
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_maximum_intensity_probability() const
    {
        return _max_probability;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_intensity_correlation(CoOccurenceDirection direction) const
    {
        auto& occurence = get_co_occurence_matrix(direction);
        float sum_of_elements = occurence.sum();

        float col_mean = 0;
        float row_mean = 0;
        float col_stddev = 0;
        float row_stddev = 0;

        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
                row_mean += i * (occurence(i, j) / sum_of_elements);

        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
                row_stddev += (i - row_mean) * (i - row_mean) * (occurence(i, j) / sum_of_elements);

        if (row_stddev == 0)
            return std::numeric_limits<float>::infinity();

        row_stddev = sqrt(row_stddev);

        for (size_t j = 0; j < occurence.cols(); ++j)
            for (size_t i = 0; i < occurence.rows(); ++i)
                col_mean += j * (occurence(i, j) / sum_of_elements);

        for (size_t j = 0; j < occurence.cols(); ++j)
            for (size_t i = 0; i < occurence.rows(); ++i)
                col_stddev += (j - col_mean) * (j - col_mean) * (occurence(i, j) / sum_of_elements);

        if (col_stddev == 0)
            return std::numeric_limits<float>::infinity();

        col_stddev = sqrt(col_stddev);

        float correlation = 0;
        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
                correlation += ((i - row_mean) * (j - col_mean) * occurence(i, j) / sum_of_elements) / (row_stddev * col_stddev);

        assert(correlation >= -1 and correlation <= 1);
        return correlation;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_uniformity(CoOccurenceDirection direction) const
    {
        auto& occurence = get_co_occurence_matrix(direction);
        size_t sum_of_elements = occurence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
            {
                float p_ij = float(occurence(i, j)) / float(sum_of_elements);
                sum += p_ij * p_ij;
            }

        return sum;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_homogenity(CoOccurenceDirection direction) const
    {
        auto& occurence = get_co_occurence_matrix(direction);
        size_t sum_of_elements = occurence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
                sum += float(occurence(i, j)) / float(sum_of_elements) / (1.f + abs(int(i) - int(j)));

        return sum;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_entropy(CoOccurenceDirection direction) const
    {
        auto& occurence = get_co_occurence_matrix(direction);
        size_t sum_of_elements = occurence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
            {
                float p_ij = float(occurence(i, j)) / float(sum_of_elements);
                sum += p_ij * log2(p_ij);
            }

        return sum / (2 * log2(256));
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_contrast(ImageRegion::CoOccurenceDirection direction) const
    {
        auto& occurence = get_co_occurence_matrix(direction);

        size_t sum_of_elements = occurence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurence.rows(); ++i)
            for (size_t j = 0; j < occurence.cols(); ++j)
            {
                sum += std::abs(int(i) - int(j)) * occurence(i, j) / float(sum_of_elements);
            }

        return sum / (255*255);
    }
}