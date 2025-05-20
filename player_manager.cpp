#include "globals.h"
#include "player.h"
#include  "player_manager.h"
#include  "enemies_controller.h"
#include "level.h"
#include "level_manager.h"
#include "graphics.h"
#include "images.h"
#include "game_state.h"
#include "physics.h"

using namespace Graphics;
using namespace Game;
using namespace Physics;

// reset plyer stats for a new game or level reset
void PlayerController::resetStats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int& score : player_level_scores) {
        score = 0;
    }
}

void PlayerController::incrementScore() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int PlayerController::getTotalScore() {
    int total = 0;
    for (int score : player_level_scores) {
        total += score;
    }
    return total;

}

void PlayerController::spawnPlayer() {
    player_y_velocity = 0;

    auto& level = LevelManager::getInstanceLevel().getCurrentLevel();
    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t col = 0; col < level.get_columns(); ++col) {
            if (Level::get_level_cell(row, col) == PLAYER) {
                Player::getInstance().setPosX(static_cast<float>(col));
                Player::getInstance().setPosY(static_cast<float>(row));
                LevelManager::setLevelCell(row, col, AIR);
                return;
            }
        }
    }
}

// Moves the player horizontally if no wall collision is detected
void PlayerController::moveHorizontally(float delta) {
    Player& player = Player::getInstance();
    float nextX = player.posX() + delta;

    if (!LevelManager::isColliding({nextX, player.posY()}, WALL)) {
        player.setPosX(nextX);
    } else {
        player.setPosX(roundf(player.posX()));
        return;
    }

    player.setLookingForward(delta > 0);
    if (delta != 0.0f) player.setMoving(true);
}

// Updates all player logic, including movement, interaction, and survival
void PlayerController::updatePlayer() {
    Player& player = Player::getInstance();
    Player::updateGravity();

    Vector2 pos = player.position();

    // Coin pickup
    if (LevelManager::isColliding(pos, COIN)) {
        LevelManager::getCollider(pos, COIN) = AIR;
        PlayerController::incrementScore();
    }

    // Exit logic
    if (LevelManager::isColliding(pos, EXIT)) {
        if (timer > 0) {
            timer -= 25;
            time_to_coin_counter += 5;
            if (time_to_coin_counter / 60 > 1) {
                incrementScore();
                time_to_coin_counter = 0;
            }
        } else {
            LevelManager::getInstanceLevel().loadLevel(1);
            PlaySound(exit_sound);
        }
    } else {
        if (timer >= 0) timer--;
    }

    // Kill if touching spike or falling out of level
    if (LevelManager::isColliding(pos, SPIKE) || pos.y > static_cast<float>(LevelManager::getInstanceLevel().getCurrentLevel().get_rows())) {
        killPlayer();
    }

    // Enemy collision
    if (EnemiesControl::getInstance().isCollidingWithEnemies(pos)) {
        if (player_y_velocity > 0) {
            EnemiesControl::getInstance().removeCollidingEnemy(pos);
            PlaySound(kill_enemy_sound);
            incrementScore();
            player_y_velocity = -BOUNCE_OFF_ENEMY;
        } else {
            killPlayer();
        }
    }
}

// Draws the player sprite depending on movement and state
void PlayerController::drawPlayer() {
    const Player& player = Player::getInstance();
    horizontal_shift = (screen_size.x - cell_size) / 2;

    Vector2 drawPos = {
        horizontal_shift,
        player.posY() * cell_size
    };

    if (game_state == State::GAME) {
        if (!player.isOnGround()) {
            Graphics::draw_image(player.isLookingForward() ? player_jump_forward_image : player_jump_backwards_image, drawPos, cell_size);
        } else if (player.isMoving()) {
            draw_sprite(player.isLookingForward() ? player_walk_forward_sprite : player_walk_backwards_sprite, drawPos, cell_size);
            Player::getInstance().setMoving(false);
        } else {
            Graphics::draw_image(player.isLookingForward() ? player_stand_forward_image : player_stand_backwards_image, drawPos, cell_size);
        }
    } else {
        Graphics::draw_image(player_dead_image, drawPos, cell_size);
    }
}

void PlayerController::killPlayer() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = State::DEATH;
    player_lives--;
    player_level_scores[level_index] = 0;
}