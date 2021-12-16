// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <julia.h>
#include <string>
#include <deque>

namespace crisp::julia
{
    class Proxy;

    struct State
    {
        friend class Proxy;

        // https://docs.julialang.org/en/v1/manual/calling-c-and-fortran-code/
        // https://docs.julialang.org/en/v1/devdocs/init/
        public:
            /// @brief inits environment
            static void init();

            static auto script(std::string);

            template<typename T>
            static Proxy register_variable(std::string n, T value);
            static Proxy get_variable(std::string name);

            static void collect_garbage();

        private:
            template<typename... T>
            static Proxy execute(T... strings);

            // garbage collection and allocation management
            static void allocate(jl_value_t*);
            static void schedule_free(jl_value_t*);

            static inline jl_value_t* _no_gc_dict_name = nullptr;
            static inline jl_function_t* _no_gc_dict_insert = nullptr;
            static inline jl_function_t* _no_gc_dict_erase = nullptr;
            static inline jl_datatype_t* _no_gc_wrapper = nullptr;

            // cleanup on exit
            static inline bool _initialized = false;
            static void at_exit()
            {
                if (_initialized)
                {
                    jl_eval_string(R"(
                    for (key, val) in _dict
                               delete!(_dict, key)
                           end
                    )");

                    jl_atexit_hook(0);
                }
            }
    };
}

#include ".src/julia_state.inl"