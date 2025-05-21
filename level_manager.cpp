#include "level_manager.h"
#include "enemies_controller.h"
#include "level.h"
#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "player_manager.h"
#include "game_state.h"
#include "graphics.h"
#include "assets.h"
#include <fstream>

using namespace Game;
using namespace Graphics;

std::vector<Level> LevelManager::LEVELS;

bool LevelManager::isInsideLevel(int row, int column) {
    const auto& level = getInstanceLevel().getCurrentLevel();
    return row >= 0 && row < static_cast<int>(level.get_rows()) &&
           column >= 0 && column < static_cast<int>(level.get_columns());
}

bool LevelManager::isColliding(Vector2 pos, char lookFor) {
    Rectangle entityHitbox = { pos.x, pos.y, 1.0f, 1.0f };

    for (int row = static_cast<int>(pos.y - 1); row < static_cast<int>(pos.y + 1); ++row) {
        for (int col = static_cast<int>(pos.x - 1); col < static_cast<int>(pos.x + 1); ++col) {
            if (!isInsideLevel(row, col)) continue;
            if (Level::get_level_cell(row, col) == lookFor) {
                Rectangle blockHitbox = { static_cast<float>(col), static_cast<float>(row), 1.0f, 1.0f };
                if (CheckCollisionRecs(entityHitbox, blockHitbox)) return true;
            }
        }
    }
    return false;
}

char& LevelManager::getCollider(Vector2 pos, char lookFor) {
    Rectangle block_hitbox = { pos.x, pos.y, 1.0f,  1.0f };

    for (int row = static_cast<int>(pos.y - 1); row < static_cast<int>(pos.y + 1); ++row) {
        for (int col = static_cast<int>(pos.x - 1); col < static_cast<int>(pos.x + 1); ++col) {
            if (!isInsideLevel(row, col)) continue;
            if (Level::get_level_cell(row, col) == lookFor) {
                Rectangle blockHitbox = { static_cast<float>(col), static_cast<float>(row), 1.0f, 1.0f };
                if (CheckCollisionRecs(block_hitbox, blockHitbox)) {
                    return Level::get_level_cell(row, col);
                }
            }
        }
    }

    return Level::get_level_cell(static_cast<size_t>(pos.x), static_cast<size_t>(pos.y));
}

void LevelManager::resetLevelIndex() {
    level_index = 0;
}

void LevelManager::loadLevel(int offset) {
    level_index += offset;

    if (level_index >= LEVEL_COUNT) {
        game_state = State::VICTORY;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    size_t rows = LEVELS[level_index].get_rows();
    size_t cols = LEVELS[level_index].get_columns();
    currentLevelData = new char[rows * cols];

    const char* sourceData = LEVELS[level_index].get_data();
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            currentLevelData[row * cols + col] = sourceData[row * cols + col];
        }
    }

    getInstanceLevel().setCurrentLevel(Level{ rows, cols, currentLevelData });
    PlayerController::spawnPlayer();
    EnemiesControl::getInstance().spawnEnemies();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void LevelManager::unloadLevel() {
    delete[] getInstanceLevel().getCurrentLevelData();
}

void LevelManager::drawLevel() {
    const Level& level = getInstanceLevel().getCurrentLevel();
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t col = 0; col < level.get_columns(); ++col) {
            Vector2 pos = {
                (static_cast<float>(col) - Player::getInstance().posX()) * cell_size + horizontal_shift,
                static_cast<float>(row) * cell_size
            };

            switch (Level::get_level_cell(row, col)) {
                case WALL:        Graphics::draw_image(wall_image, pos, cell_size); break;
                case WALL_DARK:   Graphics::draw_image(wall_dark_image, pos, cell_size); break;
                case SPIKE:       Graphics::draw_image(spike_image, pos, cell_size); break;
                case COIN:        drawSprite(coin_sprite, pos, cell_size); break;
                case EXIT:        Graphics::draw_image(exit_image, pos, cell_size); break;
                default: break;
            }
        }
    }
    PlayerController::drawPlayer();
    EnemiesControl::drawEnemies();
}

void LevelManager::setLevelCell(size_t row, size_t column, char value) {
    Level::get_level_cell(row, column) = value;
}

void LevelManager::setCurrentLevel(const Level& level) {
    currentLevel = level;
}

char& Level::get_level_cell(size_t row, size_t col) {
    const auto& level = LevelManager::getInstanceLevel().getCurrentLevel();
    return level.data[row * level.get_columns() + col];
}

Level LevelManager::parseLevelRLE(const std::string& rleData) {
    std::vector<std::string> rows;
    std::string currentRow;
    std::string counter;

    for (char c : rleData) {
        if (c == '|') {
            rows.push_back(currentRow);
            currentRow.clear();
            counter.clear();
        } else if (c == ';') {
            if (!currentRow.empty()) rows.push_back(currentRow);
            break;
        } else if (std::isdigit(c)) {
            counter += c;
        } else {
            int count = counter.empty() ? 1 : std::stoi(counter);
            currentRow.append(count, c);
            counter.clear();
        }
    }

    if (!currentRow.empty()) rows.push_back(currentRow);

    size_t rowCount = rows.size();
    size_t colCount = rows[0].length();
    char* levelData = new char[rowCount * colCount];

    for (size_t r = 0; r < rowCount; ++r) {
        for (size_t c = 0; c < colCount; ++c) {
            levelData[r * colCount + c] = rows[r][c];
        }
    }

    return Level{ rowCount, colCount, levelData };
}

std::vector<Level> LevelManager::loadLevelsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    LEVELS.clear();

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;
        LEVELS.push_back(parseLevelRLE(line));
    }

    if (LEVELS.empty()) {
        throw std::runtime_error("No valid levels were loaded from the file.");
    }

    return LEVELS;
}
