// 
// Copyright 2021 Clemens Cords
// Created on 13.11.21 by clem (mail@clemens-cords.com)
//

#include <pseudocolor_mapping.hpp>
#include <gpu_side/state.hpp>
#include <gpu_side/texture_workspace.hpp>

namespace crisp 
{
    PseudoColor<GPU_SIDE>::PseudoColor()
    {
        auto shader = State::register_shader("pseudocolor.glsl");
        _program = State::register_program(shader);
        State::free_shader(shader);
    }

    Texture<float, 3> PseudColor<GPU_SIDE>::apply(const Texture<float, 3>& in)
    {
        auto out = Texture<float, 3>(State::register_texture<float, 3>(in.get_handle()));
        auto array = State::register_array(range_mapping_to_array());

        auto workspace = out.get_workspace();
        workspace.swap_buffers();
        State::bind_shader_program(_program);
        State::bind_texture(_program, "_texture", in);
        State::bind_array(_program, "_gray_to_hue", array);
        workspace.draw_to_buffer();

        return out;
    }

    Texture<float, 3>& PseudColor<GPU_SIDE>::apply(Texture<float, 3>& in)
    {
        auto array = State::register_array(range_mapping_to_array());

        auto workspace = in.get_workspace();
        State::bind_shader_program(_program);
        State::bind_texture(_program, "_texture", in);
        State::bind_array(_program, "_gray_to_hue", array);
        workspace.display();
        workspace.yield();

        return in;
    }

    std::array<float, 256> PseudoColor<GPU_SIDE>::range_mapping_to_array() const
    {
        auto gray_to_hue = [this](float x) -> float
        {
            for (auto& map : _gray_to_hue)
            {
                float min_gray = map.first.first;
                float max_gray = map.first.second;
                float min_hue = map.second.first;
                float max_hue = map.second.second;

                if (x >= min_gray and x <= max_gray)
                {
                    float hue = x;
                    hue -= min_gray;
                    hue /= abs(max_gray - min_gray);

                    hue = project(min_hue, max_hue, hue);

                    return hue;
                }
            }

            for (auto& map : _gray_to_inverse_hue)
            {
                float min_gray = map.first.first;
                float max_gray = map.first.second;
                float min_hue = map.second.first;
                float max_hue = map.second.second;

                if (x >= min_gray and x <= max_gray)
                {
                    float hue = x;
                    hue -= min_gray;
                    hue /= abs(max_gray - min_gray);

                    hue = project(min_hue, max_hue, hue);
                    hue = max_hue - (hue - min_hue);

                    return hue;
                }
            }

            return -1;
        };

        std::array<float, 256> out = {};
        for (size_t i = 0; i < 256; ++i)
            out[i] = gray_to_hue(float(i) / 256.f);

        return out;
    }
    
    void PseudoColor<GPU_SIDE>::add_value_to_hue(float gray, float hue)
    {
        _gray_to_hue.push_back({{gray, gray}, {hue, hue}});
    }

    void PseudoColor<GPU_SIDE>::add_value_range_to_hue(float min_gray, float max_gray, float hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {hue, hue}});
    }

    void PseudoColor<GPU_SIDE>::add_value_range_to_hue_range(
            float min_gray, 
            float max_gray, 
            float min_hue,
            float max_hue)
    {
        _gray_to_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }

    void PseudoColor<GPU_SIDE>::add_value_range_to_inverse_hue_range(
            float min_gray, 
            float max_gray,
            float min_hue, 
            float max_hue)
    {
        _gray_to_inverse_hue.push_back({{min_gray, max_gray}, {min_hue, max_hue}});
    }
}