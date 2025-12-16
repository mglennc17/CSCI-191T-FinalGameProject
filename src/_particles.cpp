#include "_particles.h"
#include <cstdlib>
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846
#endif

_particles::_particles()
{
    // start inactive
    life = 0.0f;
}

_particles::~_particles()
{
}

// helper for random values between 0 and 1
static float frand01()
{
    return rand() / (float)RAND_MAX;
}

void _particles::initDust(float x, float y, float z)
{
    // spawn position
    position.x = x;
    position.y = y;
    position.z = z;

    // Start fully alive
    life = 1.0f;

    // Dust color: mostly gray/white-ish, not rainbow
    float c = 0.6f + frand01() * 0.35f;   // 0.60 .. 0.95
    color.x = c;
    color.y = c;
    color.z = c;

    // Randomized direction with a bias:
    // slightly upward (y+)
    // slightly backward (z-), since this is dust behind wheels
    float theta = frand01() * 2.0f * (float)PI;
    float speed = 0.6f + frand01() * 0.8f; // moderate variation in

    velocity.x = cos(theta) * 0.25f * speed;          // small side scatter
    velocity.y = (0.25f + frand01() * 0.6f) * speed;  // upward motion
    velocity.z = -(0.6f + frand01() * 0.8f) * speed;  // backward motion
}

void _particles::update(float dt)
{
    if (life <= 0.0f) return;

    // move particle
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    position.z += velocity.z * dt;

    // slow it down over time so it feels like dust
    velocity.x *= (1.0f - 1.5f * dt);
    velocity.y *= (1.0f - 1.0f * dt);
    velocity.z *= (1.0f - 1.5f * dt);

    // Fade out
    life -= 1.6f * dt; // ~0.6 seconds lifetime at 60fps
    if (life < 0.0f) life = 0.0f;
}
