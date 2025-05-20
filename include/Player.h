#ifndef PLAYER_H
#define PLAYER_H

#include<cmath>
#include<vector>
#include"Keletappi.h"
using std::vector;

class Player
{
    public:

        int HP;
        int damageDealt = 12;

        float projectionMatrix[16]; //(4x4 matrix)

        //x,y,z location and rotation along Y-axis only needed
        vector<float> location;
        vector<float> velocity;
        float rotationY;
        float fov;

        float a;    //acceleration for movement
        float rotateSpeed = 2.3f;
        float speed = 0.62f;
        float dX;
        float dZ;

        int handX;
        int handY;

        Player();

        void movePlayer(int v, int r);

        void applyCollision();
        void update();

        void strike();

    protected:

    private:

        //collision"box"
        float minX, maxX, minZ, maxZ;
        float grapDistance;
        float vD = 0.0f;

        bool isStriking;
        int strikeCooldown = 20;    //frames (60fps)
        int strikeTimer;

        bool checkIntersection(Keletappi* tappi);

};

#endif // PLAYER_H
