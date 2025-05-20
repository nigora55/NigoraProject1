#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "level.h"
#include "raylib.h"
#include <string>
#include <vector>

class LevelManager {
public:
    static std::vector<Level> LEVELS;

    static LevelManager& getInstanceLevel() {
        static LevelManager instance;
        return instance;
    };

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;

    [[nodiscard]] static std::vector<Level> getLevels() noexcept { return LEVELS; }
    Level& getCurrentLevel() noexcept { return currentLevel; }
    [[nodiscard]] char* getCurrentLevelData() const noexcept { return currentLevelData; }


    static bool isInsideLevel(int row, int column);
    static bool isColliding(Vector2 pos, char lookFor);
    static char& getCollider(Vector2 pos, char lookFor);

    void loadLevel(int offset = 0);
    static void resetLevelIndex();
    static void drawLevel();
    static void unloadLevel();
    static void setLevelCell(size_t row,  size_t column, char value);
    void setCurrentLevel(const Level& level);

    static std::vector<Level> loadLevelsFromFile(const std::string& filename);
    static Level parseLevelRLE(const std::string& rleData);


private:
    LevelManager() = default;
    ~LevelManager() = default;

    Level currentLevel;
    char* currentLevelData = nullptr;
};
#endif //LEVEL_MANAGER_H
