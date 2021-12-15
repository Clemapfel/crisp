// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

namespace crisp
{
    /// @brief bool proxy used as a template argument for specializations
    template<bool b>
    struct is_gpu_side
    {
        static constexpr bool value = b;
        constexpr operator bool() const noexcept {return value;}
        constexpr bool operator()() const noexcept {return value;}
    };

    /// @brief object lives in RAM, operated upon by the CPU
    using CPU_SIDE = is_gpu_side<false>;

    /// @brief object lives in VRAM, operated upon by the GPU
    using GPU_SIDE = is_gpu_side<true>;
}