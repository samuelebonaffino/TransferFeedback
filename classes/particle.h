#ifndef PARTICLE
#define PARTICLE

#include <iostream>
#include <cmath>

class Particle
{
    public:
        uint ID;
        float pos[2];
        float vel[2];

    Particle(uint id)
    {
        ID = id;
        pos[0] = randomf(-1.0f, 1.0f);
        pos[1] = randomf(-1.0f, 1.0f);
        vel[0] = 0.0f;
        vel[1] = 0.0f;
    }
    static float randomf()
    {
        return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
    static float randomf(float maxVal)
    {
        return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxVal));
    }
    static float randomf(float lo, float hi)
    {
        return lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
    }
};

#endif