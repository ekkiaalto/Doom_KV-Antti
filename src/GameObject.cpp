#include "GameObject.h"
#include "string"
#include "Engine.h"
#include "TextureManager.h"
#include "iostream"

extern Engine* engine;

GameObject::GameObject(char t, float a, float b, float c)
{
    type = t;

    x = a;
    y = b;
    z = c;

    //perkeleellinen if-jakso eri objektityypeille, kaikki kova-koodattuja
    if (type == 't'){
        minX = -2.0f;
        minZ = -2.0f;
        maxX = 2.0f;
        maxZ = 2.0f;
    }
    if (type == 's') {
        minX = -2.5f;
        minZ = -2.5f;
        maxX = 2.5f;
        maxZ = 2.5f;
    }
    if (type == 'e'){
        minX = -1.25f;
        minZ = -1.25f;
        maxX = 1.25f;
        maxZ = 1.25f;
    }
}

GameObject::~GameObject()
{
    //dtor
}

//build function for each static object type


void GameObject::render(){

    if (type == 't'){
        engine->textureManager->drawCube("assets/textures/stone.png", x,y,z,3.0f, 7.0f, 3.0f);
        engine->textureManager->drawCube("assets/textures/stone.png", x,y,z,4.0f, 1.0f, 5.0f);
        engine->textureManager->drawCube("assets/textures/stone.png", x,y+7.0f,z,4.0f, 1.0f, 3.0f);
    }
    if (type == 's'){
        engine->textureManager->drawCube("assets/textures/bricks.png", x, y, z, 5.0f, 10.0f, 2.0f);
    }
    if (type == 'e'){
        engine->textureManager->drawCube("assets/textures/electric.png", x, y, z, 2.0f, 3.5f, 8.0f);
        engine->textureManager->drawCube("assets/textures/stone.png", x,y,z,2.5f, 0.5f, 0.5f);
        engine->textureManager->drawCube("assets/textures/stone.png", x,y+3.5f,z,2.5f, 0.5f, 0.5f);
    }

}
