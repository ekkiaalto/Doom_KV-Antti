#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <vector>
#include <string>

class LevelManager
{
    public:
        LevelManager();
        virtual ~LevelManager();

        void buildLevel();
        void cleanLevel();

        int level;

        void newLevel();
        void renderLevel();

        int gambiinaCount;

    protected:

    private:
        int lastLevelIndex;
        std::vector<std::vector<char>> tileGrid;
        std::vector<std::vector<char>> objectGrid;
        std::vector<std::vector<char>> readLevel(const std::string& filename);
};

#endif // LEVELMANAGER_H
