#include "Player.h"
#include "GL/freeglut.h"
#include<vector>
#include<iostream>
#include<Engine.h>
#include"Keletappi.h"

extern Engine* engine;
vector<float> up = {0.0f, 1.0f, 0.0f};

Player::Player()
{
    location = {0.0f,1.75f,0.0f};  //init 1.75m over the floor sine 1.75 is character hight
    velocity = {0.0f,0.0f,0.0f}; //initial velocity
    rotationY = 0.0f;
    fov = 75.0f;

    HP = 100;

    minX = -1.5;
    maxX = 1.5;
    minZ = -1.5;
    maxZ = 1.5;

    dX,dZ = 0.0f, 0.0f;
    grapDistance = 2.0f;

    isStriking = false;
    strikeTimer = 0;
    handX = 760;
    handY = 220;
}

//move player and the camera among it
void Player::movePlayer(int v, int r){

    //update rotation
    rotationY = rotationY + float(r)*rotateSpeed;

    //direction camera is looking at
    dX = sin(rotationY * 3.14159f / 180.0f);
    dZ = -cos(rotationY * 3.14159f / 180.0f);

    //camera is updated in the render function


    //update the current velocity and location
    float vX = dX*speed*float(v);
    float vZ = dZ*speed*float(v);

    velocity[0] = velocity[0] + vX;
    velocity[1] = velocity[1];
    velocity[2] = velocity[2] + vZ;

    vD += std::sqrt(vX*vX + vZ*vZ)/1.5;
}

void Player::applyCollision(){

    //check collision bewteen all game objects and enemies, apply velocity when needed.

    float dx = 0.0f;
    float dz = 0.0f;

    for (auto& pair : engine->enemies) {
        //pair.first, pair.second
        //check overlap for all 4 edges

        float mX = pair.second->location[0] + pair.second->minX;
        float MX = pair.second->location[0] + pair.second->maxX;
        float mZ = pair.second->location[2] + pair.second->minZ;
        float MZ = pair.second->location[2] + pair.second->maxZ;

        if ((mX < location[0] + minX) && (location[0] + minX < MX) && (mZ < location[2] + minZ) && (location[2] + minZ < MZ)){
            dx = dx +1.0f;
            dz = dz +1.0f;
        }
        if ((mX < location[0] + minX) && (location[0] + minX < MX) && (mZ < location[2] + maxZ) && (location[2] + maxZ < MZ)){
            dx = dx +1.0f;
            dz = dz -1.0f;
        }
        if ((mX < location[0] + maxX) && (location[0] + maxX < MX) && (mZ < location[2] + minZ) && (location[2] + minZ < MZ)){
            dx = dx -1.0f;
            dz = dz +1.0f;
        }
        if ((mX < location[0] + maxX) && (location[0] + maxX < MX) && (mZ < location[2] + maxZ) && (location[2] + maxZ < MZ)){
            dx = dx -1.0f;
            dz = dz -1.0f;
        }
    }

    //wall collision
    for (auto& pair : engine->objects){

        //check overlap for all 4 edges

        float mX = pair.second->x + pair.second->minX;
        float MX = pair.second->x + pair.second->maxX;
        float mZ = pair.second->z + pair.second->minZ;
        float MZ = pair.second->z + pair.second->maxZ;

        if ((mX < location[0] + minX) && (location[0] + minX < MX) && (mZ < location[2] + minZ) && (location[2] + minZ < MZ)){
            dx = dx +1.0f;
            dz = dz +1.0f;
        }
        if ((mX < location[0] + minX) && (location[0] + minX < MX) && (mZ < location[2] + maxZ) && (location[2] + maxZ < MZ)){
            dx = dx +1.0f;
            dz = dz -1.0f;
        }
        if ((mX < location[0] + maxX) && (location[0] + maxX < MX) && (mZ < location[2] + minZ) && (location[2] + minZ < MZ)){
            dx = dx -1.0f;
            dz = dz +1.0f;
        }
        if ((mX < location[0] + maxX) && (location[0] + maxX < MX) && (mZ < location[2] + maxZ) && (location[2] + maxZ < MZ)){
            dx = dx -1.0f;
            dz = dz -1.0f;
        }
    }

    //normalize the speed and multiply with speed
    float len = std::sqrt(dx * dx + dz * dz);
    if (len != 0){
        dx = dx/len * (speed);
        dz = dz/len * (speed);
    }

    velocity[0] = velocity[0] + dx;
    velocity[2] = velocity[2] + dz;

}

void Player::update(){

    location[0] = location[0] + velocity[0];
    location[1] = 1.75f + std::sin(vD)/3;
    location[2] = location[2] + velocity[2];

    velocity = {0.0f, 0.0f, 0.0f};  //reset velocity

    //check for potions
    for (auto& pair : engine->potions){
        if (std::sqrt(std::pow(location[0]-pair.second->x,2) + std::pow(location[2]-pair.second->z,2)) < grapDistance){
            HP += 20;
            if (HP > 100){
                HP = 100;
            }
            engine->potions.erase(pair.second->index);
            engine->score += 50;
            engine->levelManager->gambiinaCount -= 1;
        }
    }

    if (isStriking){
        strikeTimer += 1;
        if (strikeTimer >= strikeCooldown){
            isStriking = false;
            strikeTimer = 0;
            handX = 720;
            handY = 220;
        }

        //move the hand to correct position on screen
        float t = 0;
        if (strikeTimer <= (strikeCooldown/2)){
            t = 2.0*float(strikeTimer)/float(strikeCooldown);
        } else {
            t = 1.0-(float(strikeTimer)/float(strikeCooldown) - 0.5f)*2;
        }

        handX = 720 + t * (520 - 720);  //lerp
        handY = 220 + t * (270 - 220);

        if (strikeTimer == strikeCooldown/2){
            //check for hits
            for (auto& pair : engine->enemies){

                if (checkIntersection(pair.second)){

                    pair.second->HP -= damageDealt;
                    pair.second->aggro = true;

                    //add particles when hit
                    pair.second->emitParticles();


                    if (pair.second->HP < 0){
                        engine->enemies.erase(pair.second->index);
                        engine->score += 100;
                    }
                }
            }
        }

    }
}

void Player::strike(){

    if (isStriking){
        return;
    }

    isStriking = true;

}

//check overlap with the axisaligned hitbox
bool Player::checkIntersection(Keletappi* tappi){


    if (dX == 0.0f) dX = 1e-6f;
    if (dZ == 0.0f) dZ = 1e-6f;

    //get box edges in world space
    float tx = tappi->location[0]+tappi->minX;
    float Tx = tappi->location[0]+tappi->maxX;
    float tz = tappi->location[2]+tappi->minZ;
    float Tz = tappi->location[2]+tappi->maxZ;

    //get linear function for comparable axis and check for overlap
    float Mx = (dX / dZ) * (Tz - location[2]) + location[0];
    float mx = (dX / dZ) * (tz - location[2]) + location[0];
    float Mz = (dZ / dX) * (Tx - location[0]) + location[2];
    float mz = (dZ / dX) * (tx - location[0]) + location[2];


    bool isHit = false;

    if (tx < Mx && Mx < Tx){
        isHit = true;
    }
    else if (tx < mx && mx < Tx){
        isHit = true;
    }else if (tz < Mz && Mz < Tz){
        isHit = true;
    } else if (tz < mz && mz < Tz){
        isHit = true;
    }

    return isHit;
}
