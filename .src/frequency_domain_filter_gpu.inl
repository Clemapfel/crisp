// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#include <frequency_domain_filter.hpp>
#include <gpu_side/state.hpp>

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
        auto tex = State::register_texture<1>(size_t(_size.x()), size_t(_size.y()), transform._spectrum);

        auto before = State::get_active_program_handle();
        State::bind_shader_program(_program);
        
        State::set_float(_program, "_cutoff", _cutoff_a);   // a equal to b for non-bandpass
        State::set_float(_program, "_cutoff_a", _cutoff_a);
        State::set_float(_program, "_cutoff_b", _cutoff_b);
        State::set_float(_program, "_pass_factor", _pass_factor);
        State::set_float(_program, "_reject_factor", _reject_factor);
        State::set_vec<2>(_program, "_offset", _offset);
        State::set_int(_program, "_order", _order);
        State::bind_texture(_program, "_texture", tex);

        State::bind_shader_program(_program);
        State::display();
        //workspace.draw_to_buffer();

        //glBindTexture(GL_TEXTURE_2D, workspace.get_buffer_texture());
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, &transform._spectrum[0]);

        // download framebuffer into transform memory
        glReadPixels(0, 0, _size.x(), _size.y(), GL_R32F, GL_FLOAT, &transform._spectrum[0]);

        State::free_texture(tex);

        // undo
        State::bind_shader_program(before);
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

        auto shader = State::register_shader("lowpass_ideal.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_lowpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("lowpass_gaussian.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_lowpass(double cutoff_frequency, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = State::register_shader("lowpass_butterworth.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_highpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("highpass_ideal.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_highpass(double cutoff_frequency, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("highpass_gaussian.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_highpass(double cutoff_frequency, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_frequency;
        _cutoff_b = cutoff_frequency;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = State::register_shader("highpass_butterworth.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_bandpass(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("bandpass_ideal.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_bandpass(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("bandpass_gaussian.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_bandpass(double cutoff_a, double cutoff_b, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = State::register_shader("bandpass_ideal.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_ideal_bandreject(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("bandreject_ideal.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_gaussian_bandreject(double cutoff_a, double cutoff_b, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = 0;

        auto shader = State::register_shader("bandreject_gaussian.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::as_butterworth_bandreject(double cutoff_a, double cutoff_b, size_t order, double pass_factor,
                                                           double reject_factor)
    {
        _cutoff_a = cutoff_a;
        _cutoff_b = cutoff_b;
        _pass_factor = pass_factor;
        _reject_factor = reject_factor;
        _order = order;

        auto shader = State::register_shader("bandreject_butterworth.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    void FrequencyDomainFilter<GPU_SIDE>::set_offset(double x_dist_from_center, double y_dist_from_center)
    {
        _offset = Vector2f{float(x_dist_from_center), float(y_dist_from_center)};
    }
}