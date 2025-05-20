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
                PlayerController::moveHorizontally(Physics::PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                PlayerController::moveHorizontally(-Physics::PLAYER_MOVEMENT_SPEED);
            }


        PlayerController::updatePlayer();

        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && Player::getInstance().isOnGround()) {
            Physics::player_y_velocity = -Physics::JUMP_STRENGTH;
        }

        EnemiesControl::getInstance().updateEnemies();

        if (LevelManager::getCollider(
        { Player::getInstance().posX(), Player::getInstance().posY() }, EXIT) == EXIT) {
            PlaySound(exit_sound);
            LevelManager::getInstanceLevel().loadLevel(1);  // load next level
            return;
        }

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
            Player::updateGravity();

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
                LevelManager::resetLevelIndex();
                PlayerController::resetStats();
                game_state = Game::State::GAME;
                LevelManager::getInstanceLevel().loadLevel();
            }
            break;

        case Game::State::VICTORY:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                LevelManager::resetLevelIndex();
                PlayerController::resetStats();
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
            LevelManager::drawLevel();
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
    LevelManager::loadLevelsFromFile("data/levels.rll");
    LevelManager::getInstanceLevel().loadLevel();

    while (!WindowShouldClose()) {
        BeginDrawing();
        update_game();
        draw_game();
        EndDrawing();
    }

    LevelManager::unloadLevel();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

