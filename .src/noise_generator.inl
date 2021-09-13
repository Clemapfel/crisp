// 
// Copyright 2021 Clemens Cords
// Created on 05.08.21 by clem (mail@clemens-cords.com)
//

#include <noise_generator.hpp>

namespace crisp
{
    template<typename Dist_t>
    NoiseGenerator<Dist_t>::NoiseGenerator(size_t seed)
        : _engine(seed)
    {}

    template<typename Dist_t>
    void NoiseGenerator<Dist_t>::set_interval(float min, float max)
    {
        _min = min;
        _max = max;
    }

    template<typename RandomNumberDistribution_t>
    size_t NoiseGenerator<RandomNumberDistribution_t>::initialize_seed()
    {
        std::random_device device;
        return device();
    }

    template<typename RandomNumberDistribution_t>
    float NoiseGenerator<RandomNumberDistribution_t>::operator()()
    {
        float value = -1;
        while (value < 0 or value > 1)
            value = _distribution(_engine);

        return std::min(_min, _max) + value * std::fabs(_max - _min) ;
    }

    float GammaNoise::operator()()
    {
        float value = -1;
        while (value < 0 or value > 1)
            value = _distribution(_engine);

        return std::min(_min, _max) + value * std::fabs(_max - _min) ;
    }

    float SaltAndPepperNoise::operator()()
    {
        float which = _distribution(_engine);

        if (which == -1)
            return std::min(_min, _max);
        else if (which == 1)
            return std::max(_min, _max);
        else
            return 0;
    }

    inline UniformNoise::UniformNoise(size_t seed)
        : NoiseGenerator<std::uniform_real_distribution<float>>(seed)
    {
        _distribution = std::uniform_real_distribution<float>(0, 1);
    }

    inline GaussianNoise::GaussianNoise(size_t seed)
        : NoiseGenerator<std::normal_distribution<float>>(seed)
    {
        _distribution = std::normal_distribution<float>(0.5, 0.5 / 3);
    }

    inline GaussianNoise::GaussianNoise(float mean, float sigma, size_t seed)
        : NoiseGenerator<std::normal_distribution<float>>(seed)
    {
        _distribution = std::normal_distribution<float>(mean, sigma);
    }

    inline GammaNoise::GammaNoise(size_t seed)
        : NoiseGenerator<std::gamma_distribution<float>>(seed)
    {
        _distribution = std::gamma_distribution<float>(3, 0.2);
    }

    inline GammaNoise::GammaNoise(float alpha, float beta, size_t seed)
        : NoiseGenerator<std::gamma_distribution<float>>(seed)
    {
        _distribution = std::gamma_distribution<float>(alpha, beta);
    }

    inline SaltAndPepperNoise::SaltAndPepperNoise(float salt_chance, float pepper_chance, size_t seed)
        : NoiseGenerator<SaltAndPepperDistribution>(seed)
    {
        _distribution = SaltAndPepperDistribution(salt_chance, pepper_chance);
    }
}