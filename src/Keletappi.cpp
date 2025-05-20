#include "Keletappi.h"
#include "Engine.h"
#include<vector>
#include<cmath>
#include "Particle.h"

extern Engine* engine;

Keletappi::Keletappi(int indx, float x, float y, float z)
{
    location = {x, y, z};

    minX = -2.5;
    maxX = 2.5;
    minZ = -2.5;
    maxZ = 2.5;

    HP = 30;
    damage = 12;
    aggro = false;
    aggroRange = 20;
    cooldown = 0;
    hitRange = 6.2;

    index = indx;

    tex = "assets/textures/keletappi.png";
    //ctor
}

Keletappi::~Keletappi()
{
    //dtor
}

void Keletappi::render(){

    engine->textureManager->drawSprite(tex, location[0], location[1], location[2], width, height);

}

void Keletappi::update(){

    //add keletappi behaviour

    float px = engine->player->location[0];
    float pz = engine->player->location[2];

    //aggro if player comes too close
    if (!aggro){
        if (std::sqrt(std::pow(location[0]-px,2) + std::pow(location[2]-pz,2)) < aggroRange){
                aggro = true;
        }
        else{
            return;
        }
    }

    if (cooldown < 30){
        cooldown++;
    }
    else{

        if (std::sqrt(std::pow(location[0]-px,2) + std::pow(location[2]-pz,2)) < hitRange){
            engine->player->HP = engine->player->HP - damage;
            cooldown = 0;
        }
    }

    //move the lazy-ass keletappi towards the player
    float nx = px - location[0];
    float nz = pz - location[2];
    float l = std::sqrt(nx * nx + nz * nz);
    if (l != 0.0f) {
        nx = nx / l;
        nz = nz / l;
    }
    location[0] += nx*0.12f;
    location[2] += nz*0.12f;
}

void Keletappi::emitParticles(){

    int totPar = 20;

    for (int n = 0; n < totPar; n++){
        engine->particles[engine->particleIndex] = new Particle(engine->particleIndex, location[0], 3.5f, location[2]);
        engine->particleIndex += 1;
    }

}
