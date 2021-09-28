// 
// Copyright 2021 Clemens Cords
// Created on 26.09.21 by clem (mail@clemens-cords.com)
//

namespace crisp
{
    template<typename Image_t>
    ImageRegion<Image_t>::Element::Element(Vector2ui position, Value_t value)
    {
        _position = position;
        _value = value;
        
        _intensity = 0;
        for (size_t i = 0; i < Value_t::size(); ++i)
            _intensity += float(value.at(i));
        
        _intensity /= Value_t::size();
    }

    template<typename Image_t>
    ImageRegion<Image_t>::ImageRegion(const ImageSegment& segment, const Image_t& image)
    {
        create_from(segment, image);
    }

    template<typename Image_t>
    void ImageRegion<Image_t>::create_from(const ImageSegment& segment, const Image_t& image)
    {
        _elements.clear();
        for (const auto& px : segment)
            _elements.insert(std::make_pair(px.to_hash(), Element(px, image(px.x(), px.y()))));

        _original_image_size = image.get_size();
        create();
    }

    template<typename Image_t>
    ImageRegion<Image_t>::ImageRegion(const Image_t& image)
    {
        create_from(image);
    }

    template<typename Image_t>
    void ImageRegion<Image_t>::create_from(const Image_t& image)
    {
        _elements.clear();
        for (size_t x = 0; x < image.get_size().x(); ++x)
            for (size_t y = 0; y < image.get_size().y(); ++y)
            {
                auto px = Vector2ui{x, y};
                _elements.insert(std::make_pair(px.to_hash(), Element(px, image(px.x(), px.y()))));
            }

        _original_image_size = image.get_size();
        create();
    }

    template<typename Image_t>
    void ImageRegion<Image_t>::create()
    {
        _min_x = std::numeric_limits<size_t>::max();
        _max_x = 0;
        _min_y = std::numeric_limits<size_t>::max();
        _max_y = 0;
        
        ImageSegment strong_pixels, weak_pixels;
        
        for (auto& pair : _elements)
        {
            const auto& px = pair.second._position;
            
            _min_x = std::min(px.x(), _min_x);
            _min_y = std::min(px.y(), _min_y);
            _max_x = std::max(px.x(), _max_x);
            _max_y = std::max(px.y(), _max_y);
            
            size_t n_unconnected = 0;
            for (int i = -1; i <= +1; ++i)
            {
                for (int j = -1; j <= +1; ++j)
                {
                    const auto new_px = Vector2i{int(px.x()) + i, int(px.y()) + j};

                    // outer edge of image is always boundary
                    if (new_px.x() + i < 0 or new_px.x() + i > _original_image_size.x()
                        or new_px.y() + j < 0 or new_px.y() + j > _original_image_size.y())
                        n_unconnected++;

                    else if (not (i == 0 and j == 0) and _elements.find(new_px.to_hash()) == _elements.end())
                        n_unconnected++;
                }
            }
         
            if (n_unconnected > 1)
                strong_pixels.insert(px);
            else if (n_unconnected == 1)
                weak_pixels.insert(px);
        }

        // trace boundary
        _boundary.clear();

        auto translate_in_direction = [&](Vector2ui c, uint8_t direction) -> Vector2ui
        {
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

                    if (to_check.x() < _min_x or to_check.x() > _max_x or
                        to_check.y() < _min_y or to_check.y() > _max_y)
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

                    if (to_check.x() < _min_x or to_check.x() > _max_x or
                        to_check.y() < _min_y or to_check.y() > _min_y)
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

        _hole_boundaries.clear();
        for (size_t i = 1; i < boundaries_out.size(); ++i)
            _hole_boundaries.push_back(boundaries_out.at(i));

        // boundary polygon
        auto turn_type = [&](size_t i_a, size_t i_b) -> int
        {
            auto point_a = _boundary.at(i_a),
                 point_b = _boundary.at(i_b);

            // warp point
            if (abs(int(point_a.x()) - int(point_b.x())) > 1 or
                abs(int(point_a.y()) - int(point_b.y())) > 1)
                return 0;

            auto dir_a = directions_out.at(0).at(i_a),
                 dir_b = directions_out.at(0).at(i_b);

            if (dir_b > dir_a or (dir_a == 7 and dir_b == 0))
                return -1; // left-hand turn
            else if (dir_b < dir_a or (dir_a == 0 and dir_b == 7))
                return +1; // right-hand turn
            else
                return 0; // colinear
        };

        _boundary_polygon.clear();
        _centroid = Vector2f{0, 0};

        for (size_t i = 0; i < _boundary.size() - 1; ++i)
        {
            _centroid += Vector2f{float(_boundary.at(i).x()), float(_boundary.at(i).y())};

            if (turn_type(i, i+1) != 0)
                _boundary_polygon.push_back(_boundary.at(i));
        }

        _centroid /= float(_boundary.size());

        // compute minor and major axis
        Eigen::Matrix<float, 2, 2> covar;

        for (auto& px : _boundary)
        {
            Eigen::Matrix<float, 2, 1> current;
            current << (px.x() - _centroid.x()),
                       (px.y() - _centroid.y());

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
    }
    
    template<typename Image_t>
    Vector2f ImageRegion<Image_t>::get_centroid() const
    {
        return _centroid;
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
            for (const auto& point : _boundary_polygon)
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
        signature.reserve(_boundary_polygon.size());

        for (auto& px : _boundary_polygon)
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
        signature.reserve(_boundary_polygon.size());

        for (auto& px : _boundary_polygon)
            signature.push_back(value(px));

        return signature;
    }

    template<typename Image_t>
    std::vector<std::complex<float>> ImageRegion<Image_t>::complex_coordinate_signature() const
    {
        std::vector<std::complex<float>> signature;
        signature.reserve(_boundary_polygon.size());

        for (auto& px : _boundary_polygon)
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
    float ImageRegion<Image_t>::get_perimeter() const
    {
        return _boundary.size();
    }

    template<typename Image_t>
    std::array<Vector2ui, 4> ImageRegion<Image_t>::get_axis_aligned_bounding_box() const
    {
        auto out = std::array<Vector2ui, 4>();

        out.at(0) = {_min_x, _min_y};
        out.at(1) = {_max_x, _min_y};
        out.at(2) = {_max_x, _max_y};
        out.at(3) = {_min_x, _max_y};

        return out;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_area() const
    {
        return _elements.size();
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
                value_sum += powf(element.position.x() - _centroid.x(), p) * 
                             powf(element.position.y() - _centroid.y(), q) * 
                             element._intensity;
            }

            return moment / powf(value_sum, (p+q) / 2.f + 1);
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
    const auto& ImageRegion<Image_t>::get_intensity_histogram() const
    {
        if (_histogram_initialized)
            return _histogram;

        std::vector<float> intensities;
        intensities.reserve(_elements.size());

        for (auto& pair : _elements)
            intensities.push_back(pair.second._intensity);

        _histogram = Histogram<QUANTIZATION_N>(intensities);
        _histogram_initialized = true;
        return _histogram;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_mean() const
    {
        if (_intensity_mean != -1)
            return _intensity_mean;

        float sum = 0;
        for (auto& pair : _elements)
            sum += pair.second._intensity;

        _intensity_mean = sum / _elements.size();
        return _intensity_mean;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_variance() const
    {
        if (_intensity_variance != -1)
            return _intensity_variance;

        const float mean = get_mean();

        float sum = 0;
        for (auto& pair : _elements)
            sum += (pair.second._intensity - mean) * (pair.second._intensity - mean);

        _intensity_variance = sum / _elements.size();
        return _intensity_variance;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_nths_moment(size_t n) const
    {
        if (n == 0)
            return 1;
        else if (n == 1)
            return 0;
        else if (n == 2)
            return 1;

        if (_nths_statistical_moment.find(n) != _nths_statistical_moment.end())
            return _nths_statistical_moment.at(n);

        const float mean = get_mean();
        const float stddev = sqrt(get_variance());

        float sum = 0;
        for (auto& pair : _elements)
            sum += pow((pair.second._intensity - mean), n);

        sum /= pow(stddev, n);
        sum /= float(_elements.size());
        _nths_statistical_moment[n] = sum;
        return sum;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_skewness() const
    {
        return get_nths_moment(3);
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_kurtosis() const
    {
        return get_nths_moment(4);
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_maximum_intensity_probability() const
    {
        if (_max_probability != -1)
            return _average_entropy;

        if (not _intensity_occurrences_initialized)
        {
            for (const auto& pair : _elements)
            {
                float intensity = pair.second._intensity;
                if (_intensity_occurrences.find(intensity) == _intensity_occurrences.end())
                    _intensity_occurrences.emplace(intensity, 1);
                else
                {
                    _intensity_occurrences.at(intensity) += 1;
                }
            }

            _intensity_occurrences_initialized = true;
        }

        size_t max_n = 0;
        float n = 0;
        for (const auto& pair : _intensity_occurrences)
        {
            max_n = std::max(pair.second, max_n);
            n++;
        }

        return max_n / n;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_average_entropy() const
    {
        if (_average_entropy != -1)
            return _average_entropy;
        
        if (not _intensity_occurrences_initialized)
        {
            for (auto& pair : _elements)
            {
                float intensity = pair.second._intensity;
                if (_intensity_occurrences.find(intensity) == _intensity_occurrences.end())
                    _intensity_occurrences.emplace(intensity, 1);
                else
                    _intensity_occurrences.at(intensity) += 1;
            }

            _intensity_occurrences_initialized = true;
        }

        float sum = 0;
        for (auto& pair : _intensity_occurrences)
        {
            float p = pair.second / float(_intensity_occurrences.size());
            sum += p * log2(p);
        }

        _average_entropy = (-1 * sum) / (2 * log2(_intensity_occurrences.size()));
        return _average_entropy;
    }
    
    template<typename Image_t>
    const auto & ImageRegion<Image_t>::get_co_occurrence_matrix(CoOccurrenceDirection direction) const
    {
        if (_co_occurrence_matrix.find(direction) != _co_occurrence_matrix.end())
            return _co_occurrence_matrix.at(direction);

        using Value_t = typename Image_t::Value_t;
        using Inner_t = typename Image_t::Value_t::Value_t;

        Eigen::MatrixXf out;
        out.resize(QUANTIZATION_N, QUANTIZATION_N);
        out.setConstant(0);

        size_t n_pairs = 0;
        auto process = [&](size_t x, size_t y, size_t x_2, size_t y_2)
        {
            auto a = _elements.find(Vector2ui{x, y}.to_hash());
            auto b = _elements.find(Vector2ui{x_2, y_2}.to_hash());

            if (a == _elements.end() or b == _elements.end())
                return;

            float a_value = 0, b_value = 0;
            for (size_t i = 0; i < Value_t::size(); ++i)
            {
                a_value += a->second._value.at(i);
                b_value += b->second._value.at(i);
            }

            a_value = (a_value / Value_t::size()) * QUANTIZATION_N;
            b_value = (b_value / Value_t::size()) * QUANTIZATION_N;

            out(size_t(a_value), size_t(b_value)) += 1;
            n_pairs += 1;
        };

        for (auto& pair : _elements)
        {
            size_t x = pair.second._position.x();
            size_t y = pair.second._position.y();

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

        for (size_t x = 0; x < out.rows(); ++x)
            for (size_t y = 0; y < out.cols(); ++y)
                out(x, y) /= float(n_pairs);

        _co_occurrence_matrix.insert(std::make_pair(direction, out));
        return _co_occurrence_matrix.at(direction);
    }
    
    template<typename Image_t>
    float ImageRegion<Image_t>::get_homogeneity(CoOccurrenceDirection direction) const
    {
        auto& occurrence = get_co_occurrence_matrix(direction);
        size_t sum_of_elements = occurrence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
                sum += occurrence(i, j) / (1.f + fabs(float(i) - float(j)));

        return sum;
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_entropy(CoOccurrenceDirection direction) const
    {
        auto& occurrence = get_co_occurrence_matrix(direction);
        size_t sum_of_elements = occurrence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
            {
                if (fabs(occurrence(i, j)) < 0.0000000001)
                    continue;

                std::cout << occurrence(i, j) << " * " << log2(occurrence(i, j)) << std::endl;
                sum += occurrence(i, j) * log2(occurrence(i, j));
            }

        return (-1 * sum) / (2 * log2(256));
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_contrast(CoOccurrenceDirection direction) const
    {
        auto& occurrence = get_co_occurrence_matrix(direction);

        size_t sum_of_elements = occurrence.sum();

        float sum = 0;
        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
            {
                sum += std::abs(int(i) - int(j)) * occurrence(i, j) / float(sum_of_elements);
            }

        return sum / (255*255);
    }

    template<typename Image_t>
    float ImageRegion<Image_t>::get_intensity_correlation(CoOccurrenceDirection direction) const
    {
        auto& occurrence = get_co_occurrence_matrix(direction);
        float sum_of_elements = occurrence.sum();

        float col_mean = 0;
        float row_mean = 0;
        float col_stddev = 0;
        float row_stddev = 0;

        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
                row_mean += i * (occurrence(i, j) / sum_of_elements);

        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
                row_stddev += (i - row_mean) * (i - row_mean) * (occurrence(i, j) / sum_of_elements);

        if (row_stddev == 0)
            return std::numeric_limits<float>::infinity();

        row_stddev = sqrt(row_stddev);

        for (size_t j = 0; j < occurrence.cols(); ++j)
            for (size_t i = 0; i < occurrence.rows(); ++i)
                col_mean += j * (occurrence(i, j) / sum_of_elements);

        for (size_t j = 0; j < occurrence.cols(); ++j)
            for (size_t i = 0; i < occurrence.rows(); ++i)
                col_stddev += (j - col_mean) * (j - col_mean) * (occurrence(i, j) / sum_of_elements);

        if (col_stddev == 0)
            return std::numeric_limits<float>::infinity();

        col_stddev = sqrt(col_stddev);

        float correlation = 0;
        for (size_t i = 0; i < occurrence.rows(); ++i)
            for (size_t j = 0; j < occurrence.cols(); ++j)
                correlation += ((i - row_mean) * (j - col_mean) * occurrence(i, j) / sum_of_elements) / (row_stddev * col_stddev);

        return correlation;
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::begin() const
    {
        _elements.begin();
    }

    template<typename Image_t>
    auto ImageRegion<Image_t>::end() const
    {
        _elements.end();
    }
}