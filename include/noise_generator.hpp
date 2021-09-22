// 
// Copyright 2021 Clemens Cords
// Created on 05.08.21 by clem (mail@clemens-cords.com)
//

#pragma once

#include <random>
#include ".src/salt_and_pepper_distribution.inl"

namespace crisp
{
    /// @brief uncorrelated noise generator
    /// @param RandomNumberDistribution_t: takes a probablitity density functions and returns a pseudo-random number following that PDF
    template<typename RandomNumberDistribution_t>
    class NoiseGenerator
    {
        public:
            /// @brief Constructor the noise generator and initialize it with a seed
            /// @param seed: (optional) the seed, if none is specified, std::random_device::operator() is called instead
            NoiseGenerator(size_t seed);

            /// @brief specificy the minimal and maximal response of the noise generator
            /// @param min: the lower bound
            /// @param max: the upper bound
            /// @notes by default the bounds are [-1, 1]
            void set_interval(float min, float max);

            /// @brief get a random value
            /// @returns a float in range [min_value, max_value]
            /// @notes the PDFs are configured such that their mean is abs(max - min) / 2
            ///       their stddev (sigma) such that: mean - 3*sigma = min and mean + 3*sigma = max.
            ///       though unlikely this means that values outisde of [min, max] can accur
            virtual float operator()();

        protected:
            float _min = -1, _max = 1;
            static size_t initialize_seed();

            std::mt19937 _engine;
            RandomNumberDistribution_t _distribution;
    };

    /// @brief generates uniformly distributed noise
    struct UniformNoise : public NoiseGenerator<std::uniform_real_distribution<float>>
    {
        /// @overload c.f. NoiseGenerator<...>::NoiseGenerator(size_t)
        UniformNoise(size_t = initialize_seed());
    };

    /// @brief generates normal distributed noise
    struct GaussianNoise : public NoiseGenerator<std::normal_distribution<float>>
    {
        /// @overload c.f. NoiseGenerator<...>::NoiseGenerator(size_t)
        GaussianNoise(size_t = initialize_seed());

        /// @brief construct guassian noise
        /// @param mean: the mean value (center of the distribution)
        /// @param sigma: the standard deviation
        /// @param seed: the seed to initialize the rng with
        GaussianNoise(float mean, float sigma, size_t seed = initialize_seed());
    };

    /// @brief generates gamma distributed noise
    struct GammaNoise : public NoiseGenerator<std::gamma_distribution<float>>
    {
        /// @overload c.f. NoiseGenerator<...>::NoiseGenerator(size_t)
        GammaNoise(size_t = initialize_seed());

        /// @brief construct gamma noise
        /// @param alpha: the shape parameter of the distribution
        /// @param beta: the scale parameter of the distribution
        /// @param seed: the seed to initialize the rng with
        GammaNoise(float shape, float scale, size_t seed = initialize_seed());

        /// @overload c.f. NoiseGenerator<...>::operator()()
        float operator()() override;
    };

    /// @brief generates salt-and-pepper noise, randomly distributed -n or n impulses
    struct SaltAndPepperNoise : public NoiseGenerator<SaltAndPepperDistribution>
    {
        /// @brief initialize the salt-and-pepper-noise generator
        /// @param salt_chance: the likelyhood of a positive impulse being returned, may be 0
        /// @param pepper_chance: the likelyhood of a negative impulse being returned, may be 0
        /// @param : the seed
        SaltAndPepperNoise(float salt_chance, float pepper_chance, size_t = initialize_seed());

        /// @overload c.f. NoiseGenerator<...>::operator()()
        /// @returns -min if pepper, +max if salt
        float operator()() override;
    };
}

#include ".src/noise_generator.inl"