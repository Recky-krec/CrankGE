#include "globals.h"

float width;
float height;
double lastX;
double lastY;
double xpos;
double ypos;
bool isLocked;
bool isEmitting;
int isLanternOn;
float lastFrame;
float currentFrame;
float deltaTime;
double FPSsum;
double FPSsum_count;

crank::Camera camera;

void initialize_globals()
{
    width = 1280.0f;
    height = 720.0f;

    lastX = 400;
    lastY = 300;

    xpos = 0;
    ypos = 0;

    lastFrame = 0;
    currentFrame = 0;
    deltaTime = 0;

    isLocked = true;
    isEmitting = false;
    isLanternOn = 1;

    FPSsum = 0;
    FPSsum_count = 0;
}


