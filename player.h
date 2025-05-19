#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "vector"

class Player {
private:
    float y_velocity;
    Vector2 pos;

    bool is_player_on_ground;
    bool is_looking_forward;
    bool is_moving;

    std::vector <int> level_scores;
    int lives;
    int level_count;
public:
    const int MAX_LIVES = 3;

    void reset_stats();
    void increment_score();
    int get_total_score();
    void kill();
    void move_horizontally(float delta);
    void update_gravity();
    void update();
    void set_pos(Vector2 pos);
    void set_y_velocity(float y_velocity);
    int get_lives();
    Vector2 get_pos();
    float get_y_velocity();

    Player (Vector2 pos, int level_count);
};


void spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);;

            if (cell == PLAYER) {
                player_pos.x = column;
                player_pos.y = row;
                set_level_cell(row, column, AIR);
                return;
            }
        }
    }
} //level


#endif //PLAYER_H