#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include"GL/freeglut.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class TextureManager
{
    public:
        TextureManager();
        virtual ~TextureManager();

        void cleanTextures();

        //return freeglut texture ID, if loaded before returns chached ID
        static GLuint loadTexture(const std::string& filename);

        void drawUIelement(const std::string& filename, int x, int y, int width, int height);
        void drawSprite(const std::string& filename, float x, float y, float z, float width, float height);
        void drawCube(const std::string& filename, float x, float y, float z, float width, float height, float texScale);
        void drawText(const std::string& text, float x, float y, void* font = GLUT_BITMAP_TIMES_ROMAN_24);
        void drawGround(const std::string& filename, float x, float z, float texScale);

    protected:

    private:
        //cache for loaded textures
        static std::unordered_map<std::string, GLuint> textureCache;

        //generate texture from the loaded file
        static GLuint generateTexture(const std::string& filename);
};

#endif // TEXTUREMANAGER_H
