#ifndef LEVEL_H
#define LEVEL_H

#include "enemies_controller.h"
#include "globals.h"

inline const char WALL      = '#',
                  WALL_DARK = '=',
                  AIR       = '-',
                  SPIKE     = '^',
                  PLAYER    = '@',
                  ENEMY     = '&',
                  COIN      = '*',
                  EXIT      = 'E';

/* Levels */

struct level {
    size_t rows = 0, columns = 0;
    std::vector <char> data;
};

class Level {
private:
   int index;
   int count;
    std::vector <level> levels;
    level current_level;
    std::vector <char> current_level_data;

public:
    char& get_level_cell(size_t row, size_t column);
    void set_level_cell(size_t row, size_t column, char chr);
    char& get_collider(Vector2 pos, char look_for);
    bool is_inside_level(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    void reset_level_index();
    void load_level(int offset = 0);
    void unload_level();

    Level (std::string path);
};


#endif //LEVEL_H