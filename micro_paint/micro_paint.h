#ifndef MICRO_PAINT_H
#define MICRO_PAINT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct drawing drawing, *Pdrawing;

struct drawing
{
    int width;
    int height;
    char *matrice;
};

typedef struct rectangle rectangle, *Prectangle;

struct rectangle 
{
    char type;
    float x;
    float y;
    float width;
    float height;
    char color;
};

#endif