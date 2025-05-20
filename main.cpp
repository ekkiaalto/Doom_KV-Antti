#include<GL/freeglut.h>
#include<iostream>
#include"Engine.h"
#define FPS 60

Engine* engine = nullptr;

int windowHeight = 960;
int windowWidth = 1280;

int fov = 45.0f;
int aspect = float(windowHeight)/float(windowWidth);
int nearplane = 1.0f;

void clean(){
    std::cout<<"clean up..."<<std::endl;

    engine -> clean();
    delete engine;

    std::cout<<"done"<<std::endl;
}

int main(int argc, char** argv){


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(100,20);
    glutCreateWindow("DOOM KV-Antti");

    //set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,16.0/9.0,1,5000);   //the perspective
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,1.0);

    //set up our "game-engine"
    engine = new Engine;

    glutDisplayFunc([](){ engine->render(); });   //set display callback function
    glutIdleFunc([](){ engine->render(); });    //idle render

    glutKeyboardFunc([](unsigned char key, int x, int y){ engine->keyInput(key, x, y); });
    glutKeyboardUpFunc([](unsigned char key, int x, int y){ engine->keyUp(key, x, y); });
    glutSpecialFunc([](int key, int x, int y){ engine->specialInput(key, x, y); });    //callback for function keys
    glutTimerFunc(16, [](int value){ engine->update(value); }, 0);
    glutSetCursor(GLUT_CURSOR_NONE);    //hide cursor

    engine -> init();

    atexit(clean);

    glutMainLoop();             //enter mainloop (exit with exit(0);)

    return 0;
}
