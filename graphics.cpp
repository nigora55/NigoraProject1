#include "graphics.h"
#include "game_state.h"
#include "player_manager.h"
#include "player.h"
#include "level_manager.h"
#include "globals.h"
#include "assets.h"
#include "utilities.h"

using namespace Graphics;
using namespace Game;

void Graphics::draw_text(const Text &text) {
    if (!text.font || text.font->texture.id == 0) return;
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(*text.font, text.str.c_str(), pos, text.size * screen_scale, text.spacing, text.color);
}




void Graphics::derive_graphics_metrics_from_loaded_level() {
    if (LevelManager::getLevels().empty()) return;

    screen_size = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    cell_size = screen_size.y / static_cast<float>(LevelManager::getInstanceLevel().getCurrentLevel().get_rows());
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;
    float max_dim = std::max(screen_size.x, screen_size.y);
    background_size = (screen_size.x > screen_size.y) ? Vector2{max_dim, max_dim / 16 * 10} : Vector2{max_dim / 10 * 16, max_dim};
    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

void Graphics::draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;
    const float offset_y = 8.0f * screen_scale;
    const float padding = 4.0f * screen_scale;
    constexpr int MAX_HEARTS_DISPLAYED = 5;

    int hearts_to_draw = std::min(player_lives, MAX_HEARTS_DISPLAYED);
    for (int i = 0; i < hearts_to_draw; ++i) {
        Vector2 pos = { padding + static_cast<float>(i) * (ICON_SIZE + padding), offset_y };
        if (heart_image.id != 0)
            Graphics::draw_image(heart_image, pos, ICON_SIZE);
    }

    // If player has more lives than screen can show, draw a multiplier label
    if (player_lives > MAX_HEARTS_DISPLAYED && menu_font.texture.id != 0) {
        std::string extra_lives_label = "x" + std::to_string(player_lives);
        Vector2 label_pos = {
            padding + static_cast<float>(hearts_to_draw) * (ICON_SIZE + padding),
            offset_y + ICON_SIZE * 0.25f
        };
        DrawTextEx(menu_font, extra_lives_label.c_str(), label_pos, ICON_SIZE * 0.5f, 1.0f, WHITE);
    }

    // Draw timer
    if (menu_font.texture.id != 0) {
        std::string timer_text = std::to_string(timer / 60);
        Vector2 timer_dim = MeasureTextEx(menu_font, timer_text.c_str(), ICON_SIZE * 0.5f, 1.0f);
        Vector2 timer_pos = {
            static_cast<float>(GetRenderWidth()) - timer_dim.x - padding,
            offset_y
        };
        DrawTextEx(menu_font, timer_text.c_str(), timer_pos, ICON_SIZE * 0.5f, 1.0f, WHITE);
    };

    // --- Draw score and coin sprite ---
    int total_score = PlayerController::getTotalScore();
    std::string score_text = std::to_string(total_score);
    Vector2 score_dim = MeasureTextEx(menu_font, score_text.c_str(), ICON_SIZE * 0.5f, 1.0f);
    Vector2 score_pos = {
        static_cast<float>(GetRenderWidth()) - score_dim.x - ICON_SIZE - padding * 2,
        offset_y + ICON_SIZE * 0.7f
    };
    DrawTextEx(menu_font, score_text.c_str(), score_pos, ICON_SIZE * 0.5f, 1.0f, WHITE);

    Vector2 coin_pos = {
        score_pos.x - ICON_SIZE - padding,
        score_pos.y
    };
    if (coin_sprite.frames && coin_sprite.frame_count > 0)
        drawSprite(coin_sprite, coin_pos, ICON_SIZE);
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

void Graphics::draw_parallax_background() {
    // Example implementation using scrolling effect
    auto scrollOffset = static_cast<float>(
    fmodf(Player::getInstance().posX() * 0.2f + static_cast<float>(game_frame) * 0.01f, background_size.x)
    );

    // Background layers loop seamlessly
    Vector2 pos1 = { -scrollOffset, background_y_offset };
    Vector2 pos2 = { background_size.x - scrollOffset, background_y_offset };

    Graphics::draw_image(background, pos1, background_size.x, background_size.y);
    Graphics::draw_image(background, pos2, background_size.x, background_size.y);
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
        Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
        Rectangle dest   = { pos.x, pos.y, width, height };
        DrawTexturePro(image, source, dest, {0, 0}, 0.0f, WHITE);
    }

    void Graphics::draw_image(const Texture2D& image, Vector2 pos, float size) {
        Graphics::draw_image(image, pos, size, size);
    }
