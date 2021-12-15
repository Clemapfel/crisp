// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>
#include <julia/julia_state.hpp>
#include <julia/julia_proxy.hpp>

namespace crisp::julia
{
    namespace detail
    {
        static inline bool JULIA_INITIALIZED = false;

        void at_exit()
        {
            if (JULIA_INITIALIZED)
                jl_atexit_hook(0);
        }
    }

    void State::init()
    {
        // hardcoded workaround, see: https://github.com/JuliaLang/julia/issues/32614#issuecomment-722706116
        jl_init_with_image("/usr/bin", NULL);
        detail::JULIA_INITIALIZED = true;

        std::atexit(&detail::at_exit);
    }

    decltype(auto) State::script(std::string str)
    {
        return Proxy(jl_eval_string(str.c_str()));
    }
}
