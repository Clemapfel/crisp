// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <image.hpp>

namespace crisp
{
    template<typename ToInner_t, size_t ToN, typename FromInner_t, size_t FromN>
    [[nodiscard]] Image<ToInner_t, ToN> convert(const Image<FromInner_t, FromN>& from);

    /* TODO
    binary -> grey
    binary -> color

    grey -> binary
    grey -> color

    color -> grey
    color -> binary
     */




}