#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace Game {
    inline int level_index = 0;
    inline constexpr int LEVEL_COUNT = 4;

    inline constexpr int MAX_LEVEL_TIME = 50 * 60;
    inline int timer = MAX_LEVEL_TIME;
    inline int time_to_coin_counter = 0;

    inline constexpr int MAX_PLAYER_LIVES = 40;
    inline int player_lives = MAX_PLAYER_LIVES;
    inline int player_level_scores[LEVEL_COUNT];

    enum class State {
        MENU,
        GAME,
        PAUSED,
        DEATH,
        GAME_OVER,
        VICTORY
    };

    inline State game_state = State::MENU;
}

#endif // GAME_STATE_H
