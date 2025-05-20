#ifndef ENGINE_H
#define ENGINE_H
#include "Player.h"
#include "GL/freeglut.h"
#include "TextureManager.h"
#include "Keletappi.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "Potion.h"
#include "Particle.h"

class Engine{
    public:
        Engine();
        virtual ~Engine();

        TextureManager *textureManager;

        void init();
        void update(int value);
        void render();
        void keyInput(unsigned char key, int x, int y);
        void keyUp(unsigned char key, int x, int y);
        void specialInput(int key, int x, int y);
        void clean();

        Player* player;

        std::unordered_map<unsigned char, bool> keyStates;

        //store all game objects for updates, physics calculations etc...
        static std::unordered_map<int, Keletappi*> enemies;
        static std::unordered_map<int, GameObject*> objects;
        static std::unordered_map<int, Potion*> potions;
        static std::unordered_map<int, Particle*> particles;

        int enemyIndex;
        int particleIndex;
        int potionIndex;
        int objectIndex;
        int score;

        LevelManager *levelManager;

    protected:

    private:

        bool onPause;

        void drawUI();

};

#endif // ENGINE_H
