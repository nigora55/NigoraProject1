#include "globals.h"
#include "player.h"
#include "graphics.h"
#include "enemies_controller.h"
#include "player_manager.h"
#include "level_manager.h"
#include "images.h"
#include "game_state.h"
#include "physics.h"

using namespace Game;

int game_frame = 0;
void update_game() {
    game_frame++;

    switch (game_state) {
        case State::MENU:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = Game::State::GAME;
                LevelManager::getInstanceLevel().loadLevel(0);
            }
            break;

        case State::GAME:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                PlayerController::getInstance().moveHorizontally(Physics::PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                PlayerController::getInstance().moveHorizontally(-Physics::PLAYER_MOVEMENT_SPEED);
            }

            Player::getInstance().setOnGround(
                LevelManager::getInstanceLevel().isColliding(
                    {Player::getInstance().posX(), Player::getInstance().posY() + 0.1f}, WALL
                )
            );

            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) &&
                Player::getInstance().isOnGround()) {
                Physics::player_y_velocity = -Physics::JUMP_STRENGTH;
            }

            PlayerController::getInstance().updatePlayer();
            EnemiesControl::getInstance().updateEnemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = Game::State::PAUSED;
            }
            break;

        case Game::State::PAUSED:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = Game::State::GAME;
            }
            break;

        case Game::State::DEATH:
            Player::getInstance().updateGravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (Game::player_lives > 0) {
                    LevelManager::getInstanceLevel().loadLevel(0);
                    game_state = Game::State::GAME;
                } else {
                    game_state = Game::State::GAME_OVER;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case Game::State::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) {
                LevelManager::getInstanceLevel().resetLevelIndex();
                PlayerController::getInstance().resetStats();
                game_state = Game::State::GAME;
                LevelManager::getInstanceLevel().loadLevel();
            }
            break;

        case Game::State::VICTORY:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                LevelManager::getInstanceLevel().resetLevelIndex();
                PlayerController::getInstance().resetStats();
                game_state = Game::State::MENU;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw_game() {


    switch (game_state) {
        case State::MENU:
            ClearBackground(BLACK);
            Graphics::draw_menu();
            break;

        case State::GAME:
            Graphics::draw_parallax_background();
            LevelManager::getInstanceLevel().drawLevel();
            Graphics::draw_game_overlay();
            break;

        case State::DEATH:
            Graphics::draw_death_screen();
            break;

        case Game::State::GAME_OVER:
            ClearBackground(BLACK);
            Graphics::draw_game_over_menu();
            break;

        case State::PAUSED:
            ClearBackground(BLACK);
            Graphics::draw_pause_menu();
            break;

        case State::VICTORY:
            Graphics::draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);
    HideCursor();

    load_fonts();
    load_images();
    load_sounds();
    LevelManager::getInstanceLevel().loadLevelsFromFile("data/levels.rll");
    LevelManager::getInstanceLevel().loadLevel();

    while (!WindowShouldClose()) {
        BeginDrawing();
        update_game();
        draw_game();
        EndDrawing();
    }

    LevelManager::getInstanceLevel().unloadLevel();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

