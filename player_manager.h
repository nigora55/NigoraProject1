#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H
class PlayerController {
public:
    // singleton access
    static PlayerController &getInstance() {
        static PlayerController instance;
        return instance;
    };

    // delete copy and move semantics
    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;
    PlayerController(PlayerController&&) = delete;
    PlayerController& operator=(PlayerController&&) = delete;

    // static methods for global game state
    static void resetStats();
    static void incrementScore();
    static int getTotalScore();

    // player behaviour
    static void moveHorizontally(float delta);
    static void spawnPlayer();
    static void drawPlayer();
    static void updatePlayer();
    static void killPlayer();

private:
    PlayerController() = default;
    ~PlayerController() = default;
};
#endif //PLAYER_MANAGER_H
