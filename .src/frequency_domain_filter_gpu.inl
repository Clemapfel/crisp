// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#include <frequency_domain_filter.hpp>
#include <gpu_side/opengl_state.hpp>

namespace crisp
{
    FrequencyDomainFilter<GPU_SIDE>::FrequencyDomainFilter(size_t width, size_t height)
        : _program(NONE), _size(Vector2f{float(width), float(height)})
    {}
    
    template<FourierTransformMode Mode>
    FrequencyDomainFilter<GPU_SIDE>::FrequencyDomainFilter(const FourierTransform<Mode>& transform)
        : FrequencyDomainFilter(transform.get_size().x(), transform.get_size().y())
    {}

    template<FourierTransformMode Mode>
    void FrequencyDomainFilter<GPU_SIDE>::apply_to(FourierTransform<Mode>& transform) const
    {
        auto tex = gl::State::register_texture<1>(size_t(_size.x()), size_t(_size.y()), transform._spectrum);

        auto before = gl::State::get_active_program_handle();
        gl::State::bind_shader_program(_program);
        
        gl::State::set_float(_program, "_cutoff", _cutoff_a);   // a equal to b for non-bandpass
        gl::State::set_float(_program, "_cutoff_a", _cutoff_a);
        gl::State::set_float(_program, "_cutoff_b", _cutoff_b);
        gl::State::set_float(_program, "_pass_factor", _pass_factor);
        gl::State::set_float(_program, "_reject_factor", _reject_factor);
        gl::State::set_vec<2>(_program, "_offset", _offset);
        gl::State::set_int(_program, "_order", _order);
        gl::State::bind_texture(_program, "_texture", tex);

        gl::State::bind_shader_program(_program);
        gl::State::display();
        //workspace.draw_to_buffer();

        //glBindTexture(GL_TEXTURE_2D, workspace.get_buffer_texture());
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, &transform._spectrum[0]);

        // download framebuffer into transform memory
        glReadPixels(0, 0, _size.x(), _size.y(), GL_R32F, GL_FLOAT, &transform._spectrum[0]);

        gl::State::free_texture(tex);

        // undo
        gl::State::bind_shader_program(before);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_identity()
    {
        _program = NONE;
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_lowpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("lowpass_ideal.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_lowpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("lowpass_gaussian.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = gl::State::register_shader("lowpass_butterworth.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_highpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("highpass_ideal.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_highpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("highpass_gaussian.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = gl::State::register_shader("highpass_butterworth.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_bandpass(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("bandpass_ideal.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_bandpass(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("bandpass_gaussian.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_bandpass(double cutoff_a, double cutoff_b, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = gl::State::register_shader("bandpass_ideal.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_bandreject(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("bandreject_ideal.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_bandreject(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = gl::State::register_shader("bandreject_gaussian.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_bandreject(double cutoff_a, double cutoff_b, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = gl::State::register_shader("bandreject_butterworth.glsl");
        _program = gl::State::register_program(shader);
        gl::State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::set_offset(double x_dist_from_center, double y_dist_from_center)
    {
        _offset = Vector2f{float(x_dist_from_center), float(y_dist_from_center)};
    }
}