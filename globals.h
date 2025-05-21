#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include <string>
#include <cstddef>

// Common utility constants
constexpr float EPSILON = 0.0001f;

// Commonly used characters (used in level parsing, shared across logic)
inline constexpr char WALL      = '#';
inline constexpr char WALL_DARK = '=';
inline constexpr char AIR       = '-';
inline constexpr char SPIKE     = '^';
inline constexpr char PLAYER    = '@';
inline constexpr char ENEMY     = '&';
inline constexpr char COIN      = '*';
inline constexpr char EXIT      = 'E';


/* Fonts */

inline Font menu_font;

/* Display Text Parameters */

struct Text {
    std::string str;
    Vector2 position = {0.50f, 0.50f};
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = &menu_font;
};

inline Text game_title = {
    "Platformer",
    {0.50f, 0.50f},
    100.0f,
    RED
};

inline Text game_subtitle = {
    "Press Enter to Start",
    {0.50f, 0.65f}
};

inline Text game_paused = {
    "Press Escape to Resume"
};

inline Text death_title = {
    "You Died!",
    {0.50f, 0.50f},
    80.0f,
    RED
};

inline Text death_subtitle = {
    "Press Enter to Try Again",
    {0.50f, 0.65f}
};

inline Text game_over_title = {
    "Game Over",
    {0.50f, 0.50f},
    120.0f,
    RED
};

inline Text game_over_subtitle = {
    "Press Enter to Restart",
    {0.50f, 0.675f}
};

inline Text victory_title = {
    "You Won!",
    {0.50f, 0.50f},
    100.0f,
    RED
};

inline Text victory_subtitle = {
    "Press Enter to go back to menu",
    {0.50f, 0.65f}
};

/* Images and Sprites */

struct sprite {
    size_t frame_count    = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index    = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

// Level Elements
inline Texture2D wall_image;
inline Texture2D wall_dark_image;
inline Texture2D spike_image;
inline Texture2D exit_image;
inline sprite coin_sprite;

// UI Elements
inline Texture2D heart_image;

// Player
inline Texture2D player_stand_forward_image;
inline Texture2D player_stand_backwards_image;
inline Texture2D player_jump_forward_image;
inline Texture2D player_jump_backwards_image;
inline Texture2D player_dead_image;
inline sprite player_walk_forward_sprite;
inline sprite player_walk_backwards_sprite;

// Enemy
inline sprite enemy_walk;

// Background Elements
inline Texture2D background;
inline Texture2D midlleground;
inline Texture2D foreground;

/* Sounds */

inline Sound coin_sound;
inline Sound exit_sound;
inline Sound player_death_sound;
inline Sound kill_enemy_sound;
inline Sound game_over_sound;

inline size_t game_frame = 0;






#endif // GLOBALS_H
