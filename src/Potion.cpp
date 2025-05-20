#include "Potion.h"
#include <string>
#include "TextureManager.h"
#include "Engine.h"
extern Engine* engine;

Potion::Potion(int ind, float a, float b, float c)
{
    tex = "assets/textures/isoG.png";
    x = a;
    y = b;
    z = c;
    r = 1.7f;
    index = ind;
}

Potion::~Potion()
{
    //dtor
}

void Potion::render(){
    engine->textureManager->drawSprite(tex, x,y,z, width, height);

}
