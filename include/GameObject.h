#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <vector>
#include <string>

//common class for all different kind of basic STATIC objects

class GameObject
{
    public:
        GameObject(char t, float a, float b, float c);
        virtual ~GameObject();
        void render();
        std::vector<float> location; //(x,y,z), center bottom origin
        char type;

        //collision"box"
        float minX, maxX, minZ, maxZ;
        float x, y, z;

    protected:

    private:


};

#endif // GAMEOBJECT_H
