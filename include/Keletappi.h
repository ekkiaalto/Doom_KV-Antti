#ifndef KELETAPPI_H
#define KELETAPPI_H
#include <string>
#include<vector>
using std::vector;

class Keletappi
{
    public:
        Keletappi(int indx, float x, float y, float z);
        virtual ~Keletappi();

        //location in x,y,z and rotation along Y-axis
        vector<float> location;  //(0,0,0)
        std::string tex;
        //update keletappi
        void update();
        void render();

        //collision"box"
        float minX, maxX, minZ, maxZ;

        int HP;
        int index;
        int damage;

        void emitParticles();
        bool aggro;

    protected:

    private:

        float aggroRange;
        int cooldown;
        float hitRange;

        float width = 3.0f;
        float height = 4.0f;
};

#endif // KELETAPPI_H
