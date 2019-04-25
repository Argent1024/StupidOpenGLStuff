#ifndef TEXTURE_H
#define TEXTURE_H

#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <map>

#include "stb_image.h"

using namespace std;

class TextureManger {
private:
    map<string, unsigned int> m;

    unsigned int loadtexture(const string& filepath) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0); 
        if( !data ) {
            cout<<"Failed to load texture"<<endl;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

public:
    int load(const string& name, const string& file) {
        if(m.find(name) != m.end()) {
            return 0;
        } else {
            cout<<"loading texture "<<name<<" "<<file<<endl;
            m[name] = loadtexture(file);
            return 0;
        }
    }

    unsigned int get(const string& name) {
        assert(m.find(name) != m.end());
        return m[name];
    }
};
#endif