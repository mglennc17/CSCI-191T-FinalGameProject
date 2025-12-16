#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <_common.h>

class _particles
{
public:
    _particles();
    virtual ~_particles();

    vec3 position;
    vec3 color;
    vec3 velocity;
    float life;      // 1.0 = full, 0.0 = dead

    void initDust(float x, float y, float z); // spawn a dust particle
    void update(float dt);

    bool isAlive() const { return life > 0.0f; }
};

#endif // _PARTICLES_H
