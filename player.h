#ifndef PLAYER_H
#define PLAYER_H

#include "level_manager.h"

class Player {
public:
    static Player& getInstance() {
        static Player instance;
        return instance;
    };

    // position getter
    [[nodiscard]] Vector2 position() const noexcept { return player_pos; }
    [[nodiscard]] float posX() const noexcept { return player_pos.x; }
    [[nodiscard]] float posY() const { return player_pos.y; }

    // position setters
    void setPosX(float x) noexcept { player_pos.x = x; }
    void setPosY(float y) noexcept { player_pos.y = y; }

    // state flags
    [[nodiscard]] bool isOnGround() const noexcept { return on_ground; }
    void setOnGround(bool value) noexcept { on_ground = value; }

    [[nodiscard]] bool isLookingForward() const noexcept { return looks_forward; }
    void setLookingForward(bool value) noexcept { looks_forward = value; }

    [[nodiscard]] bool isMoving() const noexcept { return moves; }
    void setMoving(bool value) noexcept { moves = value; }

    void updateGravity();

private:
    Player()  = default;
    ~Player() = default;

    Vector2 player_pos{0, 0};
    bool on_ground{false};
    bool looks_forward{true};
    bool moves{false};
};

#endif //PLAYER_H