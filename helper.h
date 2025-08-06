#pragma once
#include <queue>
#include <string>
#include <algorithm>
#include "glew.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "icVector.H"

const float	WHITE[] = { 1.,1.,1.,1. };
float*
Array3(float a, float b, float c);
void
SetMaterial(float r, float g, float b, float shininess);
void
SetPointLight(int ilight, float x, float y, float z, float r, float g, float b);