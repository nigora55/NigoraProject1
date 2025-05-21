#include "enemies_controller.h"
#include "level_manager.h"
#include "player.h"
#include "globals.h"
#include "physics.h"
#include "graphics.h"
#include "assets.h"
#include <algorithm>
#include <raylib.h>

using namespace Physics;
using namespace Graphics;

void EnemiesControl::spawnEnemies() {
    enemies.clear();
    const Level& level = LevelManager::getInstanceLevel().getCurrentLevel();

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t col = 0; col < level.get_columns(); ++col) {
            if (Level::get_level_cell(row, col) == ENEMY) {
                enemies.emplace_back(Vector2{static_cast<float>(col), static_cast<float>(row)}, true);
                LevelManager::setLevelCell(row, col, AIR);
            }
        }
    }
}

void EnemiesControl::updateEnemies() {
    for (auto& enemy : enemies) {
        Vector2 pos = enemy.getPos();
        float next_x = pos.x + (enemy.isLookingRight() ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);

        if (LevelManager::isColliding({next_x, pos.y}, WALL)) {
            enemy.setLookingRight(!enemy.isLookingRight());
        } else {
            enemy.setPos({next_x, pos.y});
        }
    }
}


// Custom is_colliding function for enemies
bool EnemiesControl::isCollidingWithEnemies(const Vector2 pos) const {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    return std::any_of(enemies.begin(), enemies.end(), [&](const Enemy& enemy) {
    const Rectangle enemy_hitbox = {enemy.getPos().x, enemy.getPos().y, 1.0f, 1.0f};
    return CheckCollisionRecs(entity_hitbox, enemy_hitbox);
});
}

void EnemiesControl::removeCollidingEnemy(const Vector2 pos) {
    const Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [&](const Enemy& enemy) {
            Rectangle enemy_hitbox = {enemy.getPos().x, enemy.getPos().y, 1.0f, 1.0f};
            return CheckCollisionRecs(entity_hitbox, enemy_hitbox);
        }),
        enemies.end());
}

void EnemiesControl::drawEnemies() {
    float shift = (screen_size.x - cell_size) / 2;
    const auto& enemies = EnemiesControl::getInstance().getEnemies();

    for (const auto& enemy : enemies) {
        Vector2 pos = {
            (enemy.getPos().x - Player::getInstance().posX()) * cell_size + shift,
            enemy.getPos().y * cell_size
        };
        drawSprite(enemy_walk, pos, cell_size);
    }
}
