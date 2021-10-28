// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#include <frequency_domain_filter.hpp>
#include <gpu_side/state.hpp>

namespace crisp
{
    FrequencyDomainFilter<GPU_SIDE>::FrequencyDomainFilter()
    {
        _program = NONE;
    }

    template<FourierTransformMode Mode>
    void FrequencyDomainFilter<GPU_SIDE>::apply_to(FourierTransform <Mode>&) const
    {

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