// 
// Copyright 2021 Clemens Cords
// Created on 13.09.21 by clem (mail@clemens-cords.com)
//

#include <random>

namespace crisp
{
    class SaltAndPepperDistribution
    {
        private:
            float _salt_p,
                  _pepper_p;

            std::uniform_real_distribution<float> _salt_dist,
                    _pepper_dist,
                    _tie_dist;

        public:
            SaltAndPepperDistribution() = default;

            inline SaltAndPepperDistribution(float salt_p, float pepper_p)
                    : _salt_p(salt_p), _pepper_p(pepper_p), _salt_dist(0, 1), _pepper_dist(0, 1), _tie_dist(0, 1)
            {}

            inline float operator()(std::mt19937& engine)
            {
                bool pepper = _pepper_dist(engine) <= _pepper_p;
                bool salt = _salt_dist(engine) <= _salt_p;

                if (pepper and not salt)
                    return -1.f;
                else if (salt and not pepper)
                    return 1.f;
                else if (salt and pepper)
                    return _tie_dist(engine) > 0.5 ? 1.f : -1.f;
                else
                    return 0.f;
            }

            inline void reset()
            {
                _salt_dist.reset();
                _pepper_dist.reset();
                _tie_dist.reset();
            }
    };
}