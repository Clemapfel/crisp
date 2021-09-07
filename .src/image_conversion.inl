// 
// Copyright 2021 Clemens Cords
// Created on 07.09.21 by clem (mail@clemens-cords.com)
//

#include <image/image_conversion.hpp>

namespace crisp
{
    template<typename ToInner_t, size_t ToN, typename FromInner_t, size_t FromN>
    [[nodiscard]] Image<ToInner_t, ToN> convert(const Image<FromInner_t, FromN>& from)
    {
        static_assert(ToN >= FromN,
                      "converting from one image to another one with less planes would cause planes to be discarded");

        Image<ToInner_t, ToN> to;
        to.create(from.get_size().x(), from.get_size().y());

        to.set_padding_type(from.get_padding_type());

        for (long y = 0; y < from.get_size().x(); ++y)
        {
            for (long x = 0; x < from.get_size().x(); ++x)
            {
                ToInner_t new_value = ToInner_t(0);
                FromInner_t old_value = from.at(x, y);

                for (size_t i = 0; i < FromInner_t::size(); ++i)
                    new_value.at(i) = static_cast<ToInner_t>(old_value.at(i));

                to.at(x, y) = new_value;
            }
        }

        return to;
    }
}