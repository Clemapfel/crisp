// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>
#include <string>

namespace crisp::julia
{
    struct State
    {
        // https://docs.julialang.org/en/v1/manual/calling-c-and-fortran-code/
        // https://docs.julialang.org/en/v1/devdocs/init/
        public:
            /// @brief inits environment
            static void init();

            static decltype(auto) script(std::string);

        private:
    };
}

#include ".src/julia_state.inl"