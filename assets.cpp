#include "raylib.h"
#include "globals.h"
#include "assets.h"

#include <string>
#include <cassert>

void loadFonts() {
    menu_font = LoadFontEx("data/fonts/ARCADE_N.ttf", 256, nullptr, 128);
}

void unloadFonts() {
    UnloadFont(menu_font);
}

void loadImages() {
    wall_image                   = LoadTexture("data/images/wall.png");
    wall_dark_image              = LoadTexture("data/images/wall_dark.png");
    spike_image                  = LoadTexture("data/images/spikes.png");
    exit_image                   = LoadTexture("data/images/exit.png");

    coin_sprite                  = loadSprite("data/images/coin/coin", ".png", 3, true, 18);
    heart_image                  = LoadTexture("data/images/heart.png");

    player_stand_forward_image   = LoadTexture("data/images/player_stand_forward.png");
    player_stand_backwards_image = LoadTexture("data/images/player_stand_backwards.png");
    player_jump_forward_image    = LoadTexture("data/images/player_jump_forward.png");
    player_jump_backwards_image  = LoadTexture("data/images/player_jump_backwards.png");
    player_dead_image            = LoadTexture("data/images/player_dead.png");
    player_walk_forward_sprite   = loadSprite("data/images/player_walk_forward/player", ".png", 3, true, 15);
    player_walk_backwards_sprite = loadSprite("data/images/player_walk_backwards/player", ".png", 3, true, 15);

    enemy_walk                   = loadSprite("data/images/enemy_walk/enemy", ".png", 2, true, 15);

    background                   = LoadTexture("data/images/background/background.png");
    midlleground                 = LoadTexture("data/images/background/middleground.png");
    foreground                   = LoadTexture("data/images/background/foreground.png");
}

void unloadImages() {
    UnloadTexture(wall_image);
    UnloadTexture(wall_dark_image);
    UnloadTexture(spike_image);
    UnloadTexture(exit_image);

    unloadSprite(coin_sprite);
    UnloadTexture(heart_image);

    UnloadTexture(player_stand_forward_image);
    UnloadTexture(player_stand_backwards_image);
    UnloadTexture(player_jump_forward_image);
    UnloadTexture(player_jump_backwards_image);
    UnloadTexture(player_dead_image);
    unloadSprite(player_walk_forward_sprite);
    unloadSprite(player_walk_backwards_sprite);

    unloadSprite(enemy_walk);

    UnloadTexture(background);
    UnloadTexture(midlleground);
    UnloadTexture(foreground);
}

void drawImage(Texture2D image, Vector2 pos, float size) {
    drawImage(image, pos, size, size);
}

void drawImage(Texture2D image, Vector2 pos, float width, float height) {
    Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    Rectangle destination = { pos.x, pos.y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

sprite loadSprite(
    const std::string &file_name_prefix,
    const std::string &file_name_suffix,
    size_t frame_count,
    bool loop,
    size_t frames_to_skip
) {
    assert(frame_count < 100);

    sprite result = {
        frame_count, frames_to_skip, 0, 0, loop, 0, new Texture2D[frame_count]
    };

    for (size_t i = 0; i < frame_count; ++i) {
        std::string file_name;
        if (frame_count < 10) {
            file_name = file_name_prefix;
            file_name += std::to_string(i);
            file_name += file_name_suffix;
        } else {
            file_name = file_name_prefix;
            file_name += i < 10 ? ("0" + std::to_string(i)) : std::to_string(i);
            file_name += file_name_suffix;
        }
        result.frames[i] = LoadTexture(file_name.c_str());
    }

    return result;
}

void unloadSprite(sprite &sprite) {
    assert(sprite.frames != nullptr);

    for (size_t i = 0; i < sprite.frame_count; ++i) {
        UnloadTexture(sprite.frames[i]);
    }
    delete[] sprite.frames;
    sprite.frames = nullptr;
}

void drawSprite(sprite &sprite, Vector2 pos, float size) {
    drawSprite(sprite, pos, size, size);
}

void drawSprite(sprite &sprite, Vector2 pos, float width, float height) {
    drawImage(sprite.frames[sprite.frame_index], pos, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

void loadSounds() {
    InitAudioDevice();
    coin_sound         = LoadSound("data/sounds/coin.wav");
    exit_sound         = LoadSound("data/sounds/exit.wav");
    kill_enemy_sound   = LoadSound("data/sounds/kill_enemy.wav");
    player_death_sound = LoadSound("data/sounds/player_death.wav");
    game_over_sound    = LoadSound("data/sounds/game_over.wav");
}

void unloadSounds() {
    UnloadSound(coin_sound);
    UnloadSound(exit_sound);
    UnloadSound(kill_enemy_sound);
    UnloadSound(player_death_sound);
    UnloadSound(game_over_sound);
}

