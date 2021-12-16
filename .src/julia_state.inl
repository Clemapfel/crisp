// 
// Copyright 2021 Clemens Cords
// Created on 15.12.21 by clem (mail@clemens-cords.com)

#pragma once

#include <julia.h>
#include <julia/julia_state.hpp>
#include <julia/julia_proxy.hpp>

namespace crisp::julia
{
    void State::init()
    {
        // hardcoded workaround for linux, see: https://github.com/JuliaLang/julia/issues/32614#issuecomment-722706116
        jl_init_with_image("/usr/bin", NULL);

        _no_gc_dict_name = jl_eval_string("global __refs = IdDict()");
        _no_gc_dict_insert = jl_get_function(jl_base_module, "setindex!");
        _no_gc_dict_erase = jl_get_function(jl_base_module, "delete!");
        _no_gc_wrapper =  (jl_datatype_t*) jl_eval_string("Base.RefValue{Any}");

        allocate(_no_gc_dict_name);

        detail::JULIA_INITIALIZED = true;
        std::atexit(&State::at_exit);
    }

    auto State::script(std::string str)
    {
        return execute(str);
    }

    Proxy State::get_variable(std::string name)
    {
        return execute("return ", name);
    }

    template<typename T>
    Proxy State::register_variable(std::string name, T value)
    {
        return execute(name, " = ", value);
    }

    template<typename... T>
    Proxy State::execute(T... strings)
    {
        std::stringstream str;
        (str << ... << strings);

        str << ";" << std::endl;

        auto* val = jl_eval_string(str.str().c_str());
        JL_GC_PUSH1(val);
        auto out = Proxy(val);
        JL_GC_POP();

        return out;
    }

    void State::collect_garbage()
    {
        jl_gc_collect(JL_GC_FULL);
    }

    void State::allocate(jl_value_t* in)
    {
        jl_value_t* wrapped = jl_new_struct(_no_gc_wrapper, in);

        JL_GC_PUSH1(wrapped);
        jl_call3(_no_gc_dict_insert, _no_gc_dict_name, wrapped, in);
        JL_GC_POP();
    }

    void State::schedule_free(jl_value_t* in)
    {
        jl_call2(_no_gc_dict_erase, _no_gc_dict_name, in);
    }
}
