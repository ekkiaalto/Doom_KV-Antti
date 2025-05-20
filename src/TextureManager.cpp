#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include "Engine.h"
extern Engine* engine;

//store cache IDs
std::unordered_map<std::string, GLuint> TextureManager::textureCache;

TextureManager::TextureManager()
{
    //ctor
}

TextureManager::~TextureManager()
{
    //dtor
}

void TextureManager::cleanTextures(){

    //clear loaded textures
    textureCache.clear();
}

//load textures for render
GLuint TextureManager::loadTexture(const std::string& filename){

    //is loaded by cache
    if (textureCache.find(filename) != textureCache.end()) {
        return textureCache[filename];
    }

    GLuint textureID = generateTexture(filename);
     if (textureID != 0) {
        textureCache[filename] = textureID;
    }
    return textureID;

}

//read in a png-file and generate a glut-texture
GLuint TextureManager::generateTexture(const std::string& filename) {

    //using an existing library to load in the png files!
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);

    if (!data) {
        printf("Image not found for path: %s\n", filename);
        return 0;
    }

    //generate the OpenGL texture
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    textureCache[filename] = texID;

    return texID;
}

void TextureManager::drawUIelement(const std::string& filename, int x, int y, int width, int height){

    GLint tex = loadTexture(filename);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable     (GL_BLEND);
    glEnable     (GL_COLOR_MATERIAL);

    glColor4f    (1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2i(0,1);glVertex2i(x,y);
    glTexCoord2i(1,1);glVertex2i(x+width,y);
    glTexCoord2i(1,0);glVertex2i(x+width,y+height);
    glTexCoord2i(0,0);glVertex2i(x,y+height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//draw billboard texture at given position, origin is set to bottom center
void TextureManager::drawSprite(const std::string& filename, float x, float y, float z, float width, float height){

    GLint tex = loadTexture(filename);

    float dx = engine -> player -> location[0] - 0.0f;
    float dz = engine -> player -> location[2] - 1.0f;

    float ang = atan2(dx,dz)*180.0f * 1.0f/3.14159265f;

    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(ang, 0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-width * 0.5f, 0.0f, 0.0f); // Bottom Left
        glTexCoord2f(1.0f, 1.0f); glVertex3f( width* 0.5f, 0.0f, 0.0f); // Bottom Right
        glTexCoord2f(1.0f, 0.0f); glVertex3f( width * 0.5f, height, 0.0f); // Top Right
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-width * 0.5f,  height, 0.0f); // Top Left
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void TextureManager::drawCube(const std::string& filename, float x, float y, float z, float width, float height, float texScale){

    GLint tex = loadTexture(filename);
    float hw = width / 2.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - hw, y, z + hw);
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x + hw, y, z + hw);
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x + hw, y + height, z + hw);
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x - hw, y + height, z + hw);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - hw, y, z - hw);
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x - hw, y + height, z - hw);
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x + hw, y + height, z - hw);
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x + hw, y, z - hw);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - hw, y, z - hw);
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x - hw, y, z + hw);
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x - hw, y + height, z + hw);
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x - hw, y + height, z - hw);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + hw, y, z- hw);
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x + hw, y + height, z - hw);
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x + hw, y + height, z + hw);
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x + hw, y, z + hw);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - hw, y + height, z - hw);
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x + hw, y + height, z - hw);
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x + hw, y + height, z + hw);
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x - hw, y + height, z + hw);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - hw, y, z - hw);  // bottom front
        glTexCoord2f(height/texScale, 0.0f); glVertex3f(x - hw, y, z + hw);  // bottom back
        glTexCoord2f(height/texScale, -width/texScale); glVertex3f(x + hw, y, z + hw);  // top back
        glTexCoord2f(0.0f, -width/texScale); glVertex3f(x + hw, y, z - hw);  // top front
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//draw quad for ground
void TextureManager::drawGround(const std::string& filename, float x, float z, float texScale){

    GLint tex = loadTexture(filename);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float h = 2.5;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x-h, 0.0f, z-h);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x+h, 0.0f, z-h);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x+h, 0.0f, z+h);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x-h, 0.0f, z+h);
    glEnd();


}

void TextureManager::drawText(const std::string& text, float x, float y, void* font) {
    glRasterPos2f(x, y); // Set position in orthographic coordinates
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}
