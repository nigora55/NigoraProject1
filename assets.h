#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include <cstddef>
#include <string>

struct sprite {
    size_t frame_count = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

// Images
inline Texture2D wall_image, wall_dark_image, spike_image, exit_image, heart_image;
inline Texture2D player_stand_forward_image, player_stand_backwards_image;
inline Texture2D player_jump_forward_image, player_jump_backwards_image;
inline Texture2D player_dead_image;
inline Texture2D background, middleground, foreground;

// Sprites
inline sprite coin_sprite;
inline sprite player_walk_forward_sprite, player_walk_backwards_sprite;
inline sprite enemy_walk;

// Sounds
inline Sound coin_sound, exit_sound, player_death_sound, kill_enemy_sound, game_over_sound;

// Sprite Handling
sprite load_sprite(const std::string& prefix, const std::string& suffix, size_t count = 1, bool loop = true, size_t skip = 3);
void unload_sprite(sprite &s);
void draw_sprite(sprite &s, Vector2 pos, float width, float height);
void draw_sprite(sprite &s, Vector2 pos, float size);

#endif // ASSETS_H
