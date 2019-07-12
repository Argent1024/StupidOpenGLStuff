#ifndef LOADER_H
#define LOADER_H

#include <ctime>
#include <memory>
#include <string>
#include <map>
#include <vector>   

#include "oglutil/shader.h"
#include "oglutil/texture.h"
#include "shape.h"
#include "camera.h"

extern TextureManager GameTexManger;
extern ShaderManager GameShaderManger;
extern ShapeManager GameShapeManger;
extern Camera GameCamera;

#endif