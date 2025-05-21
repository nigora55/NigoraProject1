#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "globals.h"
#include <string>
#include <vector>
#pragma once
namespace Graphics {
    inline constexpr float SCREEN_SCALE_DIVISOR = 700.0f;
    inline Vector2 screen_size;
    inline float screen_scale;
    inline float cell_size;
    inline float horizontal_shift;

    inline Vector2 background_size;
    inline float background_y_offset;

    constexpr float VICTORY_BALL_MAX_SPEED = 4.0f;
    constexpr float VICTORY_BALL_MIN_RADIUS = 5.0f;
    constexpr float VICTORY_BALL_MAX_RADIUS = 20.0f;
    constexpr unsigned char VICTORY_BALL_TRAIL_TRANSPARENCY = 100;
    inline Color VICTORY_BALL_COLOR = YELLOW;

    struct VictoryBall {
        float x, y;
        float dx, dy;
        float radius;
    };

    inline std::vector<VictoryBall> victory_balls(50);

    void draw_text(const Text &text);
    void draw_game_overlay();
    void draw_menu();
    void draw_pause_menu();
    void draw_death_screen();
    void draw_game_over_menu();
    void draw_victory_menu();
    void draw_victory_menu_background();
    void create_victory_menu_background();
    void animate_victory_menu_background();
    void draw_parallax_background();
    void derive_graphics_metrics_from_loaded_level();
    void draw_image(const Texture2D& image, Vector2 pos, float width, float height);
    void draw_image(const Texture2D& image, Vector2 pos, float size);
}

#endif
