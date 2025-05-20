#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"

class Enemy {
public:
    Enemy(const Vector2 &position, const bool facingRight)
     : pos{position}, lookingRight{facingRight} { }

    [[nodiscard]] Vector2 getPos() const noexcept { return pos; }
    [[nodiscard]] bool isLookingRight() const noexcept { return lookingRight; }

    void setPos(const Vector2& position) noexcept { pos = position; }
    void setLookingRight(bool direction) noexcept { lookingRight = direction; }

private:
    Vector2 pos{};
    bool lookingRight = true;
};

#endif //ENEMY_H