#include "level.h"

#include <fstream>

bool Level::is_inside_level(int row, int column) {
    if (row < 0 ; row >= current_level.rows) return false;
    if (column < 0 ; column >= current_level.columns) return false;
    return true;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& Level::get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_level_cell(pos.x, pos.y);;
}

void Level::reset_level_index() {
    index = 0;
}

void Level::load_level(int offset) {
    index += offset;

    // Win logic
    if (index >= count) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        index = 0;
        return;
    }

    // Level duplication
    size_t rows = levels[index].rows;
    size_t columns = levels[index].columns;
    current_level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = levels[index].data[row * columns + column];
        }
    }

    current_level = {rows, columns, current_level_data};

    // Instantiate entities
    spawn_player();
    EnemiesController::get_instance().spawn_enemies();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

//void Level::unload_level() {
 //   delete[] current_data;
//}

// Getters and setters
char& Level::get_level_cell(size_t row, size_t column) {
    return current_level.data[row * current_level.columns + column];
}

void Level::set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

Level::Level (std::string path) {
    std::ifstream file (path);
    std::string line;
    int count = 0;
    int row_size, column_size;
    while (std::getline(file, line)) {
        if (line[0] == ';') {
            count++;
            continue;
        }
        std::string current_number;
        std::vector <char> row(0);
        for (int i = 0; i < line.length(); i++ ) {
            if (std::isdigit(line[i])) {
                current_number += line[i];
                continue;
            }
            if (current_number.empty()) {
                row.push_back(line[i]);
                continue;
            }
            int n = std::stoi(current_number);

        }
    }
};
