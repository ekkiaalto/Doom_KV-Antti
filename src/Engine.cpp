#include "Engine.h"
#include<GL/freeglut.h>
#include<iostream>
#include<random>
#include "TextureManager.h"
#include "GL/freeglut.h"
#include<vector>
#include "Keletappi.h"
#include "GameObject.h"
#include "Potion.h"
#include "Particle.h"

TextureManager* textureManager;
LevelManager* levelManager;
extern Engine* engine; //actual object

std::unordered_map<int, Keletappi*> Engine::enemies;
std::unordered_map<int, GameObject*> Engine::objects;
std::unordered_map<int, Potion*> Engine::potions;
std::unordered_map<int, Particle*> Engine::particles;

Engine::Engine()
{
    //ctor
}

Engine::~Engine()
{
    //dtor
}

void Engine::init(){


    enemyIndex = 0;
    particleIndex = 0;
    potionIndex = 0;
    objectIndex = 0;
    score = 0;

    textureManager = new TextureManager();
    player = new Player();

    //load in all the UI textures so no need to repeat it

    //on pause by default
    onPause = true;

    //instantiate first level and levelmanager etc...
    levelManager = new LevelManager();
}

void Engine::render(){

    //clear window + depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();   //reset transformation

    //update camera

    gluLookAt(
        player->location[0], player->location[1], player->location[2],
        player->location[0] + player->dX, player->location[1], player->location[2] + player->dZ,    //where to look at
        0, 1, 0     //up-vector
            );

    //stuff to render here!!!

    //add skybox
    float s = 2200.0f;
    textureManager->drawCube("assets/textures/skybox.png", 0.0f, -s/2, 0.0f, s, s, s/2);

    //static level rendering
    levelManager->renderLevel();

    //draw all keletappis
    for (auto& pair : enemies) {
        pair.second->render();
    }
    for (auto& pair : potions) {
        pair.second -> render();
    }
    for (auto& pair : particles){
        pair.second -> render();
    }


    //epilepsiatesti (ei hyvä)
    //glClearColor(((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), ((double) rand() / (RAND_MAX)), 1.0f);

    //draw UI last
    drawUI();


    glFlush(); //ensure all openGL commands are executed
    glutSwapBuffers();
    glutPostRedisplay();
}

void Engine::drawUI(){

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, 1280, 0, 960);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    //draw UI stuff here

    if (!onPause){
        textureManager -> drawUIelement("assets/textures/panel.png", 0, 0, 1280, 220);
        textureManager -> drawUIelement("assets/textures/kekkonen.png", 20, 20, 150, 150);
        textureManager -> drawUIelement("assets/textures/stabHand.png", player->handX, player->handY, 200, 300);
        int barWidth = lround(320 * float(player->HP) / 100.0f);
        textureManager -> drawUIelement("assets/textures/HP.png", 200, 40, barWidth, 80);
        textureManager -> drawUIelement("assets/textures/HPbar.png",200, 40, 320, 80);

        textureManager -> drawText("Score: " + std::to_string(score), 550.0f, 40.0f);
        textureManager -> drawText("Level: " + std::to_string(levelManager->level), 750.0f, 40.0f);
        textureManager -> drawText("Gambiinaa jaljella: " + std::to_string(levelManager->gambiinaCount), 550.0f, 80.0f);
    }
    else {
        if (player->HP > 0){
            //startScreen
            textureManager -> drawUIelement("assets/textures/aloituskuva.png", 0, 0, 1280, 960);
            textureManager -> drawText("Keraa kaikki gambiinat!", 430.0f, 550.0f);
            textureManager -> drawText("Paina valilyontia aloittaaksesi...", 430.0f, 530.0f);
        }
        else {
            textureManager -> drawText("Vihaiset Lohjalaiset varastivat gambiinat \n ja gokous on peruttu :(", 430.0f, 550.0f);
            textureManager -> drawText("Paina valilyontia yrittaaksesi uudestaan...", 430.0f, 510.0f);
        }
    }

    // Restore states
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}

void Engine::keyInput(unsigned char key, int x, int y){

    if (key == 27) { // exc key ASCII 27
        std::cout << "Deleting system32 ..." << std::endl;
        exit(0);
    }

        //update key state for movement
        if (key == 97 || key == 100 || key == 115 || key == 119 || key == 32) {
            keyStates[key] = true;
        }

}

void Engine::keyUp(unsigned char key, int x, int y) {
        if (key == 97 || key == 100 || key == 115 || key == 119 || key == 32) {
            keyStates[key] = false;  // Mark the key as released
        }
    }

void Engine::specialInput(int key, int x, int y){

    //add shift etc... if needed

}

void Engine::update(int value){


    glutPostRedisplay();            //request render()
    glutTimerFunc(16, [](int val) { engine->update(val); }, value);   //calls itself every 16ms (~60 fps)

    if (onPause){
        //start game or retry
        if (keyStates[32]){
            score = 0;
            player->location[0] = 0.0f;
            player->location[2] = 0.0f;
            player->HP = 100;
            levelManager->level = 0;
            levelManager->newLevel();
            onPause = false;
        }
        return;
    }

    //update player position based on input
    int v = 0;
    int r = 0;

    if (keyStates[97]) {  // a
        r = -1;
    }
    if (keyStates[100]) { // d
        r = 1;
    }
    if (keyStates[115]) { // s
        v = -1;
    }
    if (keyStates[119]) { // w
        v = 1;
    }
    if (keyStates[32])  { // space
        player->strike();
    }

    player->movePlayer(v, r);
    player->applyCollision();
    player->update();

    //update enemies
    for (auto& pair : enemies) {
        pair.second->update();
    }
    for (auto& pair : particles) {
        pair.second->update();
    }

    if (levelManager->gambiinaCount <= 0){
        levelManager->newLevel();
    }

    if (player->HP < 0){
        onPause = true;
    }
}

//deleta all variables etc... in game
void Engine::clean(){

    textureManager -> cleanTextures();
    delete textureManager;
    delete levelManager;
    delete player;

}
