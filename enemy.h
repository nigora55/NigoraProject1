#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"

class Enemy
{
public:
    Enemy(const Vector2 &pos, const bool is_looking_right):
        pos{pos}, looking_right{is_looking_right} { }

    [[nodiscard]] Vector2 get_pos() const
    {
        return pos;
    }

    [[nodiscard]] bool is_looking_right() const
    {
        return looking_right;
    }

    void set_pos(const Vector2& pos)
    {
        this->pos = pos;
    }

    void set_looking_right(bool looking_right)
    {
        this->looking_right = looking_right;
    }

private:
    Vector2 pos;
    bool looking_right;
};

#endif //ENEMY_H