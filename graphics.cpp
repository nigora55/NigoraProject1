#include "graphics.h"
#include "game_state.h"
#include "images.h"
#include "player_manager.h"
#include "level_manager.h"
#include "utilities.h"

using namespace Graphics;
using namespace Game;


void Graphics::draw_text(const Text &text) {
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, text.size * screen_scale, text.spacing, text.color);
}

void Graphics::derive_graphics_metrics_from_loaded_level() {
    screen_size = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    cell_size = screen_size.y / static_cast<float>(LevelManager::getInstanceLevel().getCurrentLevel().get_rows());
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;
    float max_dim = std::max(screen_size.x, screen_size.y);
    background_size = (screen_size.x > screen_size.y) ? Vector2{max_dim, max_dim / 16 * 10} : Vector2{max_dim / 10 * 16, max_dim};
    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

void Graphics::draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;
    float offset_y = 8.0f * screen_scale;

    for (int i = 0; i < player_lives; i++) {
        Graphics::draw_image(heart_image, {ICON_SIZE * static_cast<float>(i) + 4.0f * screen_scale, offset_y}, ICON_SIZE);
    }

    Vector2 time_dim = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(),
               {(static_cast<float>(GetRenderWidth()) - time_dim.x) * 0.5f, offset_y}, ICON_SIZE, 2.0f, WHITE);

    int total_score = PlayerController::getTotalScore();
    Vector2 score_dim = MeasureTextEx(menu_font, std::to_string(total_score).c_str(), ICON_SIZE, 2.0f);
    Vector2 score_pos = {static_cast<float>(GetRenderWidth()) - score_dim.x - ICON_SIZE, offset_y};
    DrawTextEx(menu_font, std::to_string(total_score).c_str(), score_pos, ICON_SIZE, 2.0f, WHITE);
    draw_sprite(coin_sprite, {static_cast<float>(GetRenderWidth()) - ICON_SIZE, offset_y}, ICON_SIZE);
}

void Graphics::draw_menu() {
    draw_text(game_title);
    draw_text(game_subtitle);
}

void Graphics::draw_pause_menu() {
    draw_text(game_paused);
}

void Graphics::draw_death_screen() {
    draw_victory_menu_background();
    LevelManager::drawLevel();
    draw_game_overlay();
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), {0, 0, 0, 100});
    draw_text(death_title);
    draw_text(death_subtitle);
}

void Graphics::draw_game_over_menu() {
    draw_text(game_over_title);
    draw_text(game_over_subtitle);
}

void Graphics::create_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x  = rand_up_to(screen_size.x);
        ball.y  = rand_up_to(screen_size.y);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED) * screen_scale;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED) * screen_scale;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS) * screen_scale;
    }
    ClearBackground(BLACK);
    EndDrawing(); BeginDrawing(); ClearBackground(BLACK); EndDrawing(); BeginDrawing();
}

void Graphics::animate_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x += ball.dx;
        if (ball.x - ball.radius < 0 || ball.x + ball.radius >= screen_size.x) ball.dx = -ball.dx;
        ball.y += ball.dy;
        if (ball.y - ball.radius < 0 || ball.y + ball.radius >= screen_size.y) ball.dy = -ball.dy;
    }
}

void Graphics::draw_victory_menu_background() {
    for (auto &ball : victory_balls) {
        DrawCircleV({ball.x, ball.y}, ball.radius, VICTORY_BALL_COLOR);
    }
}

void Graphics::draw_victory_menu() {
    DrawRectangle(0, 0, static_cast<int>(screen_size.x), static_cast<int>(screen_size.y), {0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY});
    animate_victory_menu_background();
    draw_victory_menu_background();
    draw_text(victory_title);
    draw_text(victory_subtitle);
}

void Graphics::draw_image(const Texture2D& image, Vector2 pos, float width, float height) {
    DrawTexturePro(image, {0, 0, static_cast<float>(image.width), static_cast<float>(image.height)},
                   {pos.x, pos.y, width, height}, {0, 0}, 0, WHITE);
}

void Graphics::draw_image(const Texture2D& image, Vector2 pos, float size) {
    Graphics::draw_image(image, pos, size, size);
}