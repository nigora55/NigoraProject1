#ifndef ENEMIES_CONTROLLER_H
#define ENEMIES_CONTROLLER_H

#include <vector>
#include <raylib.h>

#include "enemy.h"

class EnemiesControl {
public:
    [[nodiscard]] std::vector<Enemy> get_enemies() {
        return enemies;
    }

    static EnemiesControl &get_instance() //function to create one array of enemies by returning 1 instance, return by ref since no copies are allowed for singleton
    {
        static EnemiesControl instance; //static variable becomes global variable, but it is reachable just inside its scope
        return instance;
    }

    EnemiesControl(const EnemiesControl& other)            = delete; //delete copy/move constructors since no copies are allowed for singleton
    EnemiesControl(EnemiesControl&& other)                 = delete; //create copy/move following the rule of 5
    EnemiesControl& operator=(const EnemiesControl& other) = delete;
    EnemiesControl& operator=(EnemiesControl&& other)      = delete;

    [[nodiscard]] std::vector<Enemy> get_enemies() const
    {
        return enemies;
    }

    void spawn_enemies();
    void update_enemies();
    bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);
    static void draw_enemies();

private:
    EnemiesControl()  = default;
    ~EnemiesControl() = default;
    std::vector<Enemy> enemies{ };
};



#endif //ENEMIES_CONTROLLER_H