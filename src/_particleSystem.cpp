#include "_particleSystem.h"
#include <GL/gl.h>

_particleSystem::_particleSystem(int maxParticles)
{
    if (maxParticles < 1) maxParticles = 1;
    particles.resize(maxParticles);
    nextIndex = 0;
}

_particleSystem::~_particleSystem()
{
}

void _particleSystem::emitDust(float x, float y, float z, int count)
{
    if (count < 1) return;

    const int n = (int)particles.size();
    for (int i = 0; i < count; ++i)
    {
        particles[nextIndex].initDust(x, y, z);
        nextIndex = (nextIndex + 1) % n;
    }
}

void _particleSystem::update(float dt)
{
    for (auto &p : particles)
    {
        if (p.isAlive())
            p.update(dt);
    }
}

void _particleSystem::draw() const
{
    // Draw as points (simple + fast)
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glPointSize(6.0f);

    glBegin(GL_POINTS);
    for (const auto &p : particles)
    {
        if (!p.isAlive()) continue;
        glColor4f(p.color.x, p.color.y, p.color.z, p.life);
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
