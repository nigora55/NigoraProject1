#ifndef IMAGES_H
#define IMAGES_H

#include "raylib.h"
#include <string>
#include <cstddef>

struct sprite {
    size_t frame_count;
    size_t frames_to_skip;
    size_t frames_skipped;
    size_t frame_index;
    bool loop;
    size_t prev_game_frame;
    Texture2D* frames;
};

// Image resources
extern Texture2D wall_image, wall_dark_image, spike_image, exit_image;
extern Texture2D heart_image;
extern Texture2D player_stand_forward_image, player_stand_backwards_image;
extern Texture2D player_jump_forward_image, player_jump_backwards_image;
extern Texture2D player_dead_image;
extern Texture2D background, middleground, foreground;

// Sprites
extern sprite player_walk_forward_sprite;
extern sprite player_walk_backwards_sprite;
extern sprite enemy_walk;
extern sprite coin_sprite;

// Sounds
extern Sound coin_sound, exit_sound, player_death_sound, kill_enemy_sound, game_over_sound;

// Font
extern Font menu_font;

// Functions
void load_fonts();
void unload_fonts();
void load_images();
void unload_images();
void draw_image(const Texture2D& image, Vector2 pos, float size);
void draw_image(const Texture2D& image, Vector2 pos, float width, float height);
sprite load_sprite(const std::string& prefix, const std::string& suffix, size_t frame_count, bool loop, size_t skip);
void unload_sprite(sprite& spr);
void draw_sprite(sprite& spr, Vector2 pos, float size);
void draw_sprite(sprite& spr, Vector2 pos, float width, float height);
void load_sounds();
void unload_sounds();

#endif // IMAGES_H
