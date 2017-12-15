#pragma once

#include "Graphics/Camera.h"

extern float width;
extern float height;
extern double lastX;
extern double lastY;
extern double xpos;
extern double ypos;
extern float lastFrame;
extern float currentFrame;
extern float deltaTime;
extern double FPSsum;
extern double FPSsum_count;


// Mouse input globals and cursor
extern bool isLocked;
extern bool isEmitting;
extern int isLanternOn;

extern Camera camera;

void initialize_globals();


