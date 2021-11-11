// 
// Copyright 2021 Clemens Cords
// Created on 10.11.21 by clem (mail@clemens-cords.com)
//

#include <gpu_side/hardware_accelerated_matrix.hpp>

namespace crisp
{
    HardwareAcceleratedMatrix::HardwareAcceleratedMatrix(size_t n_rows, size_t n_cols, float value)
        : _n_rows(n_rows),
          _n_cols(n_cols),
          _texture(State::register_texture<float, 1>(n_rows, n_cols, std::vector<float>(n_rows * n_cols, value)))
    {
        init();
    }

    HardwareAcceleratedMatrix::HardwareAcceleratedMatrix(const Eigen::MatrixXf& matrix)
        : _n_rows(matrix.rows()),
          _n_cols(matrix.cols()),
          _texture(State::register_texture<float, 1>(matrix.rows(), matrix.cols(), matrix.data()))
    {
        init();
    }

    HardwareAcceleratedMatrix::HardwareAcceleratedMatrix(GLNativeHandle texture)
    {
        auto info = State::get_texture_info(texture);
        _n_rows = info.width;
        _n_cols = info.height;
        _texture = Texture<float, 1>(texture);
    }

    HardwareAcceleratedMatrix::HardwareAcceleratedMatrix(const HardwareAcceleratedMatrix& other)
        : _n_rows(other._n_rows), _n_cols(other._n_cols), _texture(other._texture.get_handle())
    {
        init();
    }

    HardwareAcceleratedMatrix & HardwareAcceleratedMatrix::operator=(const HardwareAcceleratedMatrix& other)
    {
        _n_rows = other._n_rows;
        _n_cols = other._n_cols;
        _texture = Texture<float, 1>(other._texture.get_handle());

        return *this;
    }

    HardwareAcceleratedMatrix & HardwareAcceleratedMatrix::operator=(HardwareAcceleratedMatrix&& other) noexcept
    {
        _n_rows = other._n_rows;
        _n_cols = other._n_cols;
        _texture = std::move(other._texture);

        return *this;
    }

    size_t HardwareAcceleratedMatrix::rows() const
    {
        return _n_rows;
    }

    size_t HardwareAcceleratedMatrix::cols() const
    {
        return _n_cols;
    }

    void HardwareAcceleratedMatrix::create_from_concatenate_vertical(const HardwareAcceleratedMatrix& matrix,
                                                                       size_t n)
    {
        std::vector<float> data;
        data.resize(n * matrix._n_rows * matrix._n_cols);

        auto cache = matrix.get_data();
        for (size_t i = 0; i < n; ++i)
            data.insert(data.end(), cache.begin(), cache.end());

        _texture = Texture<float, 1>(State::register_texture<float, 1>(matrix._n_rows, matrix._n_cols * n, data));
    }

    void HardwareAcceleratedMatrix::create_from_concatenate_horizontal(const HardwareAcceleratedMatrix& matrix,
                                                                       size_t n)
    {
        assert(false && "debug this");

        std::vector<float> data;
        data.resize(n * matrix._n_rows * matrix._n_cols);

        auto cache = matrix.get_data();
        for (size_t i = 0; i < n; ++i)
            for (size_t col_i = 0; col_i < matrix.rows(); ++col_i)
                data.insert(data.end(), cache.begin() + col_i * matrix.cols(), cache.begin() + (col_i + 1) * matrix.cols());

        _texture = Texture<float, 1>(State::register_texture<float, 1>(matrix._n_rows * n, matrix._n_cols, data));
    }

    std::vector<float> HardwareAcceleratedMatrix::get_data() const
    {
        return State::get_texture_data(_texture.get_handle());
    }

    void HardwareAcceleratedMatrix::init()
    {
        if (not (_product == -1 or _ewise_sum == -1 or _ewise_product == -1 or _ewise_divide == -1 or _scalar_sum == -1 or _scalar_product == -1 or _transpose == -1))
            return;

        glGenFramebuffers(1, &_buffer);

        auto shader = State::register_shader("matrix_operation/ewise_add.glsl");
        _ewise_sum = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/ewise_product.glsl");
        _ewise_product = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/ewise_divide.glsl");
        _ewise_divide = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/scalar_add.glsl");
        _scalar_sum = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/scalar_product.glsl");
        _scalar_product = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/product.glsl");
        _product = State::register_program(shader);
        State::free_shader(shader);

        shader = State::register_shader("matrix_operation/transpose.glsl");
        _transpose = State::register_program(shader);
        State::free_shader(shader);
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator+(float scalar) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_scalar_sum);
        State::bind_texture(_scalar_sum, "_left", _texture.get_handle(), 0);
        State::set_float(_scalar_sum, "_scalar", scalar);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());
        
        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator+=(float scalar)
    {
        _buffer_texture = _texture;

        State::bind_shader_program(_scalar_sum);
        State::bind_texture(_scalar_sum, "_left", _buffer_texture.get_handle(), 0);
        State::set_float(_scalar_sum, "_scalar", scalar);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.get_handle(), 0);

        glBindTexture(GL_TEXTURE_2D, _buffer_texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator-(float scalar) const
    {
        return operator+(-1.f * scalar);
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator-=(float scalar)
    {
        return operator+=(-1.f * scalar);
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator*(float scalar) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_scalar_product);
        State::bind_texture(_scalar_product, "_left", _texture.get_handle(), 0);
        State::set_float(_scalar_product, "_scalar", scalar);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator*=(float scalar)
    {
        _buffer_texture = _texture;

        State::bind_shader_program(_scalar_product);
        State::bind_texture(_scalar_product, "_left", _buffer_texture.get_handle(), 0);
        State::set_float(_scalar_product, "_scalar", scalar);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.get_handle(), 0);

        glBindTexture(GL_TEXTURE_2D, _buffer_texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator/(float scalar) const
    {
        return operator*(1.f / scalar);
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator/=(float scalar)
    {
        return operator*=(1.f / scalar);
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator+(const HardwareAcceleratedMatrix& other) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_ewise_sum);
        State::bind_texture(_ewise_sum, "_left", _texture.get_handle(), 2);
        State::bind_texture(_ewise_sum, "_right", other._texture.get_handle(), 1);
        State::set_float(_ewise_sum, "_right_scalar", +1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator+=(const HardwareAcceleratedMatrix& other)
    {
        _buffer_texture = _texture;

        if (other._texture.get_handle() != _texture.get_handle())
        {
            State::bind_shader_program(_ewise_sum);
            State::bind_texture(_ewise_sum, "_left", _buffer_texture.get_handle(), 2);
            State::bind_texture(_ewise_sum, "_right", other._texture.get_handle(), 1);
            State::set_float(_ewise_sum, "_right_scalar", +1.f);

            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

            glBindTexture(GL_TEXTURE_2D, NONE);
            glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

            State::display();

            return *this;
        }

        // edge case: += with itself
        // allocate on more texture to avoid _texture being in read and draw buffer
        auto additional_buffer = _texture;

        State::bind_shader_program(_ewise_sum);
        State::bind_texture(_ewise_sum, "_left", _buffer_texture.get_handle(), 2);
        State::bind_texture(_ewise_sum, "_right", additional_buffer.get_handle(), 1);
        State::set_float(_ewise_sum, "_right_scalar", +1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator-(const HardwareAcceleratedMatrix& other) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_ewise_sum);
        State::bind_texture(_ewise_sum, "_left", _texture.get_handle(), 2);
        State::bind_texture(_ewise_sum, "_right", other._texture.get_handle(), 1);
        State::set_float(_ewise_sum, "_right_scalar", -1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator-=(const HardwareAcceleratedMatrix& other)
    {
        _buffer_texture = _texture;

        if (other._texture.get_handle() != _texture.get_handle())
        {
            State::bind_shader_program(_ewise_sum);
            State::bind_texture(_ewise_sum, "_left", _buffer_texture.get_handle(), 2);
            State::bind_texture(_ewise_sum, "_right", other._texture.get_handle(), 1);
            State::set_float(_ewise_sum, "_right_scalar", -1.f);

            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

            glBindTexture(GL_TEXTURE_2D, NONE);
            glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

            State::display();

            return *this;
        }

        // edge case: -= with itself
        auto additional_buffer = _texture;

        State::bind_shader_program(_ewise_sum);
        State::bind_texture(_ewise_sum, "_left", _buffer_texture.get_handle(), 2);
        State::bind_texture(_ewise_sum, "_right", additional_buffer.get_handle(), 1);
        State::set_float(_ewise_sum, "_right_scalar", -1.f);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator*(const HardwareAcceleratedMatrix& other) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_ewise_product);
        State::bind_texture(_ewise_product, "_left", _texture.get_handle(), 2);
        State::bind_texture(_ewise_product, "_right", other._texture.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator*=(const HardwareAcceleratedMatrix& other)
    {
        _buffer_texture = _texture;

        if (other._texture.get_handle() != _texture.get_handle())
        {
            State::bind_shader_program(_ewise_product);
            State::bind_texture(_ewise_product, "_left", _buffer_texture.get_handle(), 2);
            State::bind_texture(_ewise_product, "_right", other._texture.get_handle(), 1);

            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

            glBindTexture(GL_TEXTURE_2D, NONE);
            glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

            State::display();

            return *this;
        }

        // edge case
        auto additional_buffer = _texture;

        State::bind_shader_program(_ewise_product);
        State::bind_texture(_ewise_product, "_left", _buffer_texture.get_handle(), 2);
        State::bind_texture(_ewise_product, "_right", additional_buffer.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator/(const HardwareAcceleratedMatrix& other) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_ewise_divide);
        State::bind_texture(_ewise_divide, "_left", _texture.get_handle(), 2);
        State::bind_texture(_ewise_divide, "_right", other._texture.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator/=(const HardwareAcceleratedMatrix& other)
    {
        _buffer_texture = _texture;

        if (other._texture.get_handle() != _texture.get_handle())
        {
            State::bind_shader_program(_ewise_divide);
            State::bind_texture(_ewise_divide, "_left", _buffer_texture.get_handle(), 2);
            State::bind_texture(_ewise_divide, "_right", other._texture.get_handle(), 1);

            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

            glBindTexture(GL_TEXTURE_2D, NONE);
            glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

            State::display();

            return *this;
        }

        // edge case
        auto additional_buffer = _texture;

        State::bind_shader_program(_ewise_divide);
        State::bind_texture(_ewise_divide, "_left", _buffer_texture.get_handle(), 2);
        State::bind_texture(_ewise_divide, "_right", additional_buffer.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::operator^(const HardwareAcceleratedMatrix& other) const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().x(), _texture.get_size().y());

        State::bind_shader_program(_product);
        State::bind_texture(_product, "_left", _texture.get_handle(), 2);
        State::bind_texture(_product, "_right", other._texture.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return out;
    }

    HardwareAcceleratedMatrix& HardwareAcceleratedMatrix::operator^=(const HardwareAcceleratedMatrix& other)
    {
        _buffer_texture = _texture;

        if (other._texture.get_handle() != _texture.get_handle())
        {
            State::bind_shader_program(_product);
            State::bind_texture(_product, "_left", _buffer_texture.get_handle(), 2);
            State::bind_texture(_product, "_right", other._texture.get_handle(), 1);

            glActiveTexture(GL_TEXTURE0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
            glBindTexture(GL_TEXTURE_2D, _texture);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

            glBindTexture(GL_TEXTURE_2D, NONE);
            glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

            State::display();

            return *this;
        }

        // edge case
        auto additional_buffer = _texture;

        State::bind_shader_program(_product);
        State::bind_texture(_product, "_left", _buffer_texture.get_handle(), 2);
        State::bind_texture(_product, "_right", additional_buffer.get_handle(), 1);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

        glBindTexture(GL_TEXTURE_2D, NONE);
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        return *this;
    }

    HardwareAcceleratedMatrix HardwareAcceleratedMatrix::transpose() const
    {
        auto out = HardwareAcceleratedMatrix(_texture.get_size().y(), _texture.get_size().x());

        State::bind_shader_program(_transpose);
        State::bind_texture(_transpose, "_left", _texture.get_handle(), 2);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out._texture);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out._texture, 0);

        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();
        return out;
    }

    void HardwareAcceleratedMatrix::transpose_in_place()
    {
        auto out = State::register_texture<float, 1>(_texture.get_size().y(), _texture.get_size().x());

        State::bind_shader_program(_transpose);
        State::bind_texture(_transpose, "_left", _texture.get_handle(), 2);

        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer);
        glBindTexture(GL_TEXTURE_2D, out);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out, 0);

        glBindTexture(GL_TEXTURE_2D, _texture.get_handle());
        glViewport(0, 0, _texture.get_size().x(), _texture.get_size().y());

        State::display();

        auto old = _texture.swap_native_objects(out);
        State::free_texture(old);
    }
}

