// 
// Copyright 2021 Clemens Cords
// Created on 28.10.21 by clem (mail@clemens-cords.com)
//

#pragma once

namespace crisp
{
    template<bool b>
    struct is_gpu_side
    {
        static constexpr bool value = b;
        constexpr operator bool() const noexcept {return value;}
        constexpr bool operator()() const noexcept {return value;}
    };

    using CPU_SIDE = is_gpu_side<false>;
    using GPU_SIDE = is_gpu_side<true>;
}