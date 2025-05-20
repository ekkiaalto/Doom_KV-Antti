#ifndef PARTICLE_H
#define PARTICLE_H
#include<vector>
#include<iostream>

class Particle
{
    public:
        Particle(int ind, int x, int y, int z);
        virtual ~Particle();

        void update();
        void render();
        int index;

    protected:

    private:

        int randI(int m, int M);
        float randF(float m, float M);


        std::string tex;

        std::vector<float> location;
        std::vector<float> velocity;

        float g = -0.007f;
        float friction = 0.05f;

        float minVel = 0.03f;
        float maxVel = 0.45f;

        float width = 0.48f;

        int lifetime;
};

#endif // PARTICLE_H
