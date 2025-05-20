#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>
#include <vector>

namespace Graphics {
    inline const float SCREEN_SCALE_DIVISOR = 700.0f;
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

    struct Text {
        std::string str;
        Vector2 position = {0.5f, 0.5f};
        float size = 32.0f;
        Color color = WHITE;
        float spacing = 4.0f;
        Font* font = nullptr;
    };

    // Fonts and UI text
    inline Font menu_font;
    inline Text game_title = {"Platformer", {0.5f, 0.5f}, 100.0f, RED, 4.0f, &menu_font};
    inline Text game_subtitle = {"Press Enter to Start", {0.5f, 0.65f}};
    inline Text game_paused = {"Press Escape to Resume"};
    inline Text death_title = {"You Died!", {0.5f, 0.5f}, 80.0f, RED};
    inline Text death_subtitle = {"Press Enter to Try Again", {0.5f, 0.65f}};
    inline Text game_over_title = {"Game Over", {0.5f, 0.5f}, 120.0f, RED};
    inline Text game_over_subtitle = {"Press Enter to Restart", {0.5f, 0.675f}};
    inline Text victory_title = {"You Won!", {0.5f, 0.5f}, 100.0f, RED};
    inline Text victory_subtitle = {"Press Enter to go back to menu", {0.5f, 0.65f}};

    // Functions
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
    void derive_graphics_metrics_from_loaded_level();
    void draw_image(const Texture2D& image, Vector2 pos, float width, float height);
    void draw_image(const Texture2D& image, Vector2 pos, float size);
}

#endif
