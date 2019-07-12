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

class TextureManager {
private:
	std::map<std::string, unsigned int> m;

    unsigned int loadtexture(const std::string& filepath) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0); 
        if( !data ) {
			std::cout<<"Failed to load texture"<< std::endl;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
		return texture;
    }

public:
    int load(const std::string& name, const std::string& file) {
        if(m.find(name) != m.end()) {
            return 0;
        } else {
			std::cout<<"loading texture "<<name<<" "<<file<< std::endl;
            m[name] = loadtexture(file);
            return 0;
        }
    }

    unsigned int get(const std::string& name) {
        assert(m.find(name) != m.end());
        return m[name];
    }
};
#endif