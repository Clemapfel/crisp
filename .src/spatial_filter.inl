// 
// Copyright 2021 Clemens Cords
// Created on 09.09.21 by clem (mail@clemens-cords.com)
//

#include <spatial_filter.hpp>

namespace crisp
{
    template<typename Image_t>
    SpatialFilter<Image_t>::SpatialFilter()
    {
        set_kernel(identity(1));
        set_evaluation_function(convolution());
    }

    template<typename Image_t>
    float SpatialFilter<Image_t>::operator()(long x, long y) const
    {
        return _kernel(x, y);
    }

    template<typename Image_t>
    float& SpatialFilter<Image_t>::operator()(long x, long y)
    {
        return _kernel(x, y);
    }

    template<typename Image_t>
    void SpatialFilter<Image_t>::set_kernel(Kernel kernel)
    {
        _kernel = kernel;
    }
}