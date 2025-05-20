#include "Particle.h"
#include <random>
#include <math.h>
#include <vector>
#include <iostream>
#include "Engine.h"
#include <unordered_map>

extern Engine* engine;

Particle::Particle(int ind, int x, int y, int z)
{
    velocity = {0.0f,0.0f,0.0f};
    location = {0.0f,0.0f,0.0f};

    tex = "assets/textures/particle1.png";
    index = ind;

    //set initial velocity
    float r = randF(0.0f, 2 * 3.14f);
    float rX = sin(r);
    float rZ = -cos(r);

    r = randF(minVel, maxVel);
    rX *= r;
    rZ *= r;

    velocity[0] = rX;
    velocity[0] = randF(-0.05f, 0.3f);
    velocity[2] = rZ;

    location[0] = x;
    location[1] = y;
    location[2] = z;

    lifetime = randI(20, 40);
}

Particle::~Particle()
{
    //dtor
}

void Particle::render(){
    engine->textureManager->drawSprite(tex, location[0],location[1],location[2], width, width);
}

void Particle::update(){

    location[0] += velocity[0];
    location[1] += velocity[1];
    location[2] += velocity[2];

    velocity[0] -= velocity[0]*friction;
    velocity[1] += g;
    velocity[2] -= velocity[2]*friction;

    lifetime -= 1;
    if (lifetime < 0){
        engine->particles.erase(index);
    }
}

int Particle::randI(int m, int M) {
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(m, M);
    return dist(gen);
}

float Particle::randF(float m, float M) {
    static std::random_device rd;
    static std::mt19937 gen(rd());  // Mersenne Twister engine

    std::uniform_real_distribution<float> dist(m, M);
    return dist(gen);
}
