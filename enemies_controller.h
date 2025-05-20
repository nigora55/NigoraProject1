#ifndef ENEMIES_CONTROLLER_H
#define ENEMIES_CONTROLLER_H

#include <vector>
#include <raylib.h>
#include "enemy.h"

class EnemiesControl {
public:
    static EnemiesControl &getInstance() {
        static EnemiesControl instance; //static variable becomes global variable, but it is reachable just inside its scope
        return instance;
    }

    EnemiesControl(const EnemiesControl& other)            = delete; //delete copy/move constructors since no copies are allowed for singleton
    EnemiesControl(EnemiesControl&& other)                 = delete; //create copy/move following the rule of 5
    EnemiesControl& operator=(const EnemiesControl& other) = delete;
    EnemiesControl& operator=(EnemiesControl&& other)      = delete;

    void spawnEnemies();
    void updateEnemies();
    void removeCollidingEnemy(Vector2 pos);

    [[nodiscard]] std::vector<Enemy> getEnemies() { return enemies; }
    [[nodiscard]] bool isCollidingWithEnemies(Vector2 pos) const;

    static void drawEnemies();

private:
    EnemiesControl()  = default;
    ~EnemiesControl() = default;

    std::vector<Enemy> enemies{};
};



#endif //ENEMIES_CONTROLLER_H