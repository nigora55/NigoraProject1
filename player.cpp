#include "player.h"
#include "globals.h"
#include "physics.h"

using namespace Physics;

void Player::updateGravity() {
    Player& player = Player::getInstance();

    // 1. Check for ceiling collision while moving up
    if (LevelManager::isColliding({player.posX(), player.posY() - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // 2. Apply gravity
    player.player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    // 3. Check for ground collision
    player.on_ground = LevelManager::isColliding({player.posX(), player.posY() + 1.0f}, WALL);

    if (player.on_ground && player_y_velocity >= 0.0f) {
        player_y_velocity = 0.0f;
        player.player_pos.y = floorf(player.posY());
    }

    player.setOnGround(player.on_ground);
}



