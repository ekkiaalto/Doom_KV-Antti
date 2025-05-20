#ifndef POTION_H
#define POTION_H

#include <string>

class Potion
{
    public:
        Potion(int ind, float x, float y, float z);
        virtual ~Potion();
        float x,y,z;
        float r;
        void render();
        int index;
    protected:

    private:

        float width = 0.8f;
        float height = 2.2f;

        std::string tex;


};

#endif // POTION_H
