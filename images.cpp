#include "images.h"
#include "graphics.h"
#include "game_state.h"
#include "globals.h"
#include "raylib.h"
#include <cassert>
#include <sstream>

using namespace Graphics;
using namespace Game;

// Global resource definitions
Texture2D wall_image, wall_dark_image, spike_image, exit_image;
Texture2D heart_image;
Texture2D player_stand_forward_image, player_stand_backwards_image;
Texture2D player_jump_forward_image, player_jump_backwards_image;
Texture2D player_dead_image;
Texture2D background, middleground, foreground;

sprite player_walk_forward_sprite;
sprite player_walk_backwards_sprite;
sprite enemy_walk;
sprite coin_sprite;

Sound coin_sound, exit_sound, player_death_sound, kill_enemy_sound, game_over_sound;


void load_fonts() {
    Graphics::menu_font = LoadFontEx("data/fonts/ARCADE_N.ttf", 256, nullptr, 128);
}

void unload_fonts() {
    UnloadFont(Graphics::menu_font);
}

void load_images() {
    wall_image = LoadTexture("data/images/wall.png");
    wall_dark_image = LoadTexture("data/images/wall_dark.png");
    spike_image = LoadTexture("data/images/spikes.png");
    exit_image = LoadTexture("data/images/exit.png");

    coin_sprite = load_sprite("data/images/coin/coin", ".png", 3, true, 18);
    heart_image = LoadTexture("data/images/heart.png");

    player_stand_forward_image = LoadTexture("data/images/player_stand_forward.png");
    player_stand_backwards_image = LoadTexture("data/images/player_stand_backwards.png");
    player_jump_forward_image = LoadTexture("data/images/player_jump_forward.png");
    player_jump_backwards_image = LoadTexture("data/images/player_jump_backwards.png");
    player_dead_image = LoadTexture("data/images/player_dead.png");

    player_walk_forward_sprite = load_sprite("data/images/player_walk_forward/player", ".png", 3, true, 15);
    player_walk_backwards_sprite = load_sprite("data/images/player_walk_backwards/player", ".png", 3, true, 15);

    enemy_walk = load_sprite("data/images/enemy_walk/enemy", ".png", 2, true, 15);

    background = LoadTexture("data/images/background/background.png");
    middleground = LoadTexture("data/images/background/middleground.png");
    foreground = LoadTexture("data/images/background/foreground.png");
}

void unload_images() {
    UnloadTexture(wall_image);
    UnloadTexture(wall_dark_image);
    UnloadTexture(spike_image);
    UnloadTexture(exit_image);

    unload_sprite(coin_sprite);
    UnloadTexture(heart_image);

    UnloadTexture(player_stand_forward_image);
    UnloadTexture(player_stand_backwards_image);
    UnloadTexture(player_jump_forward_image);
    UnloadTexture(player_jump_backwards_image);
    UnloadTexture(player_dead_image);

    unload_sprite(player_walk_forward_sprite);
    unload_sprite(player_walk_backwards_sprite);

    unload_sprite(enemy_walk);

    UnloadTexture(background);
    UnloadTexture(middleground);
    UnloadTexture(foreground);
}

void draw_image(const Texture2D& image, Vector2 pos, float size) {
    ::draw_image(image, pos, size, size);
}

void draw_image(const Texture2D& image, Vector2 pos, float width, float height) {
    Rectangle source = {0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height)};
    Rectangle dest = {pos.x, pos.y, width, height};
    DrawTexturePro(image, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
}

sprite load_sprite(const std::string& prefix, const std::string& suffix, size_t frame_count, bool loop, size_t skip) {
    assert(frame_count < 100);
    sprite spr = { frame_count, skip, 0, 0, loop, 0, new Texture2D[frame_count] };

    for (size_t i = 0; i < frame_count; ++i) {
        std::string index = (frame_count < 10) ? std::to_string(i) : (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
        std::ostringstream ss;
        ss << prefix << index << suffix;
        std::string filename = ss.str();
        spr.frames[i] = LoadTexture(filename.c_str());
    }
    return spr;
}

void unload_sprite(sprite& spr) {
    if (!spr.frames) return;  // Prevent crash if already null or never allocated

    for (size_t i = 0; i < spr.frame_count; ++i) {
        if (spr.frames[i].id > 0) { // Only unload if the texture was actually loaded
            UnloadTexture(spr.frames[i]);
        }
    }

    delete[] spr.frames;
    spr.frames = nullptr;

    // Reset other fields for safety (optional)
    spr.frame_count = 0;
    spr.frame_index = 0;
    spr.frames_skipped = 0;
    spr.frames_to_skip = 0;
    spr.loop = false;
    spr.prev_game_frame = 0;
}


void draw_sprite(sprite& spr, Vector2 pos, float size) {
    draw_sprite(spr, pos, size, size);
}

void draw_sprite(sprite& spr, Vector2 pos, float width, float height) {
    if (!spr.frames || spr.frame_count == 0) return;
    if (spr.frame_index >= spr.frame_count) spr.frame_index = 0;

    Graphics::draw_image(spr.frames[spr.frame_index], pos, width, height);

    if (spr.prev_game_frame == game_frame) return;

    if (spr.frames_skipped < spr.frames_to_skip) {
        spr.frames_skipped++;
    } else {
        spr.frames_skipped = 0;
        spr.frame_index++;
        if (spr.frame_index >= spr.frame_count) {
            spr.frame_index = spr.loop ? 0 : spr.frame_count - 1;
        }
    }
    spr.prev_game_frame = game_frame;
}

void load_sounds() {
    InitAudioDevice();
    coin_sound = LoadSound("data/sounds/coin.wav");
    exit_sound = LoadSound("data/sounds/exit.wav");
    kill_enemy_sound = LoadSound("data/sounds/kill_enemy.wav");
    player_death_sound = LoadSound("data/sounds/player_death.wav");
    game_over_sound = LoadSound("data/sounds/game_over.wav");
}

void unload_sounds() {
    UnloadSound(coin_sound);
    UnloadSound(exit_sound);
    UnloadSound(kill_enemy_sound);
    UnloadSound(player_death_sound);
    UnloadSound(game_over_sound);
}

