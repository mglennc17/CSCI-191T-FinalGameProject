#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include <vector>
#include "_particles.h"

class _particleSystem
{
public:
    _particleSystem(int maxParticles = 600);
    virtual ~_particleSystem();

    // emits a short burst (used for A/D/S)
    void emitDust(float x, float y, float z, int count = 18);

    void update(float dt);
    void draw() const;

private:
    std::vector<_particles> particles;
    int nextIndex; // ring-buffer index for reusing particles
};

#endif // _PARTICLE_SYSTEM_H
