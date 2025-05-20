#include "player.h"
#include "globals.h"

void Player::updateGravity() {
    Player& player = Player::getInstance();

    // Ceiling collision: bounce downward if hitting from below
    if (LevelManager::isColliding({player.posX(), player.posY() - 0.1f}, WALL) &&
        player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
        }

    // Apply gravity to the player's vertical position
    player.player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    // Ground collision: reset velocity or snap to ground
    player.setOnGround(LevelManager::isColliding({player.posX(), player.posY() + 0.1f}, WALL));
    if (player.isOnGround()) {
        player_y_velocity = 0.0f;
        player.player_pos.y = roundf(player.player_pos.y);
    }
}
