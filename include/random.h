#pragma once

#include <random>

namespace geo{
    
    class RandomEngine{
    public:
        RandomEngine(){
            SetSeed(std::random_device{}());
        }

        RandomEngine(size_t seed){
            SetSeed(seed);
        }

        void SetSeed(size_t seed){
            rng.seed(seed);
        }

        template <typename T>
        T Uniform(T min, T max){
            assert(min < max);
            std::uniform_real_distribution<T> dist{min, max};
            return dist(rng);
        }

        template <typename T>
        T Uniform(){
            return Uniform<T>(0, 1);
        }

        template <typename T>
        T Normal(T mu, T sigma){
            std::normal_distribution<T> dist{mu, sigma};
            return dist(rng);
        }

        template <typename T>
        T Normal(){
            return Normal<T>(0, 1);
        }

    private:
        std::default_random_engine rng;
    };

}