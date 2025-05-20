#ifndef PHYSICS_H
#define PHYSICS_H

namespace Physics {
    constexpr float PLAYER_MOVEMENT_SPEED = 0.1f;
    constexpr float JUMP_STRENGTH         = 0.25f;
    constexpr float CEILING_BOUNCE_OFF    = 0.05f;
    constexpr float ENEMY_MOVEMENT_SPEED  = 0.07f;
    constexpr float BOUNCE_OFF_ENEMY      = 0.1f;
    constexpr float GRAVITY_FORCE         = 0.01f;

   inline float player_y_velocity = 0.0f;
}

#endif
