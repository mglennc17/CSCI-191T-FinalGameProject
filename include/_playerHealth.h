#ifndef _PLAYERHEALTH_H
#define _PLAYERHEALTH_H


class _playerHealth
{
    public:
         _playerHealth()
        {
            hitsTaken = 0;
            maxHits   = 3;          // between 3 and 5 allowed
            isHit     = false;
            hitTimer  = 0.0;
        }

        // Call this when a collision is detected
        void registerHit()
        {
            // Only start a new hit window if we're not already in one
            if (!isHit) {
                ++hitsTaken;
                isHit    = true;
                hitTimer = 1.0;   // 1 second of invincibility after each hit
            }
        }

        // Call once per frame with a small dt
        void update(float dt)
        {
            if (isHit) {
                hitTimer -= dt;
                if (hitTimer <= 0.0) {
                    hitTimer = 0.0;
                    isHit    = false;
                }
            }
        }

        bool isFlashing() const { return isHit; }
        bool isDead()     const { return hitsTaken >= maxHits; }

        virtual ~_playerHealth(){};

        void reset()
        {
            hitsTaken = 0;
            isHit     = false;
            hitTimer  = 0.0f;
        }

    protected:

    private:
        int   hitsTaken;
        int   maxHits;
        bool  isHit;
        float hitTimer;
};

#endif // _PLAYERHEALTH_H
