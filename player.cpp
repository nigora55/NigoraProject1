#include "player.h"
#include "enemies_controller.h"

void Player::reset_stats() {
    lives = MAX_LIVES;
    for (int i = 0; i < level_count; i++) {
        level_scores[i] = 0;
    }
}

void Player::increment_score() {
    PlaySound(coin_sound);
    level_scores[level_index]++;
}

int Player::get_total_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += level_scores[i];
    }

    return sum;
}

void Player::kill() {
    // Decrement a life and reset all collected coins in the current level
    //PlaySound(player_death_sound)
    //game_state = DEATH_STATE;
    lives--;
    level_scores[level_index] = 0;
}

void Player::move_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = pos.x + delta;
    if (!is_colliding({next_x, pos.y}, WALL)) {
        pos.x = next_x;
    }
    else {
        pos.x = roundf(pos.x);
        return;
    }

    // For drawing player animations
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void Player::update_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (is_colliding({pos.x, pos.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    pos.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_player_on_ground = is_colliding({pos.x, pos.y + 0.1f}, WALL);
    if (is_player_on_ground) {
        y_velocity = 0;
        pos.y = roundf(pos.y);
    }
}

void Player::update() {
    update_player_gravity();

    // Interacting with other level elements
    if (is_colliding(pos, COIN)) {
        get_collider(pos, COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (is_colliding(pos, EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (is_colliding(pos, SPIKE) || pos.y > current_level.rows) {
        kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemiesController::get_instance().is_colliding_with_enemies(pos)) {
        // ...check if their velocity is downwards...
        if (y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            EnemiesController::get_instance().remove_colliding_enemy(pos);
            PlaySound(kill_enemy_sound);

            increment_player_score();
            y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            kill_player();
        }
    }
}

Player::Player(Vector2 pos, int level_count) {
    this->pos = pos;
    this->level_count = level_count;
    y_velocity = 0;
    level_scores = std::vector <int> (level_count, 0);
    lives = MAX_LIVES;
}

