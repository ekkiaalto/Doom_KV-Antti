#include "LevelManager.h"
#include "Engine.h"
#include "Keletappi.h"
#include "unordered_map"
#include "Potion.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TextureManager.h"

extern Engine* engine;

LevelManager::LevelManager()
{
    level = 0;

    newLevel();
}

LevelManager::~LevelManager()
{
    //dtor
}

void LevelManager::newLevel(){

    level++;

    //total 3 levels
    if (level > 3){
        level = 1;
    }

    cleanLevel();
    buildLevel();

}

//each level is composed of ground layer and object layer (separate tables)
//level is 5x5 meter slabs with total size of 32x32 slabs (160x160)
//objects are placed to such grid
void LevelManager::buildLevel(){

    tileGrid.clear();
    objectGrid.clear();

    gambiinaCount = 0;

    //looping levelselection
    tileGrid = readLevel("assets/levels/ground" + std::to_string(level) + ".txt");
    objectGrid = readLevel("assets/levels/level" + std::to_string(level) + ".txt");


    //gameobjects
    for (size_t row = 0; row < objectGrid.size(); ++row){
        for (size_t col = 0; col < objectGrid[row].size(); ++col){

            char ch = objectGrid[row][col];

            //object position in map
            float x = (float(row)*5.0)-(16.0*5.0);
            float z = (float(col)*5.0)-(16.0*5.0);

            if (ch == 'g'){
                engine->potions[engine->potionIndex] = new Potion(engine->potionIndex, x, 0.0f, z);
                engine->potionIndex++;

                gambiinaCount++;
            }
            if (ch == 'k'){
                engine->enemies[engine->enemyIndex] = new Keletappi(engine->enemyIndex, x, 0.0f, z);
                engine->enemyIndex++;
            }

            if (ch == 't'){
                engine->objects[engine->objectIndex] = new GameObject(ch, x, 0.0f, z);
                engine->objectIndex++;
            }
            if (ch == 's'){
                engine->objects[engine->objectIndex] = new GameObject(ch, x, 0.0f, z);
                engine->objectIndex++;
            }
            if (ch == 'e'){
                engine->objects[engine->objectIndex] = new GameObject(ch, x, 0.0f, z);
                engine->objectIndex++;
            }

        }

    }
}

//draw static level objects and ground
void LevelManager::renderLevel(){

    //render ground tiles
    for (size_t row = 0; row < tileGrid.size(); ++row) {
        for (size_t col = 0; col < tileGrid[row].size(); ++col) {
            char ch = tileGrid[row][col];

            //position for tile
            float x = (float(row)*5.0)-(16.0*5.0);
            float z = (float(col)*5.0)-(16.0*5.0);


            //decoder for populating the ground tiles
            if (ch == 'k'){
                engine->textureManager->drawGround("assets/textures/kukkanen.png", x, z, 1.0f);
            }
            if (ch == 'g'){
                engine->textureManager->drawGround("assets/textures/gravel.png", x, z, 2.0f);
            }
        }
    }

    //render structures

    for (auto& pair : engine->objects){
        pair.second -> render();
    }

}

void LevelManager::cleanLevel(){

    for (auto& pair : engine->potions) {
        delete pair.second;
    }
    engine->potions.clear();
    for (auto& pair : engine->objects) {
        delete pair.second;
    }
    engine->objects.clear();

}

//for reading in level files (space as delimitter, chars for designations)
std::vector<std::vector<char>> LevelManager::readLevel(const std::string& filename) {
    std::vector<std::vector<char>> grid;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open level-file: " << filename << "\n";
        return grid;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char ch : line) {
            if (ch != ' ') {
                row.push_back(ch);
            }
        }
        grid.push_back(row);
    }

    file.close();
    return grid;
}

