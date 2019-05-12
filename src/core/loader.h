#ifndef LOADER_H
#define LOADER_H

#include <ctime>
#include <memory>
#include <string>
#include <map>
#include <vector>   
using namespace std;

#include "oglutil/shader.h"
#include "oglutil/texture.h"
#include "shape.h"

extern TextureManager GameTexManger;
extern ShaderManager GameShaderManger;
extern ShapeManager GameShapeManger;

#endif