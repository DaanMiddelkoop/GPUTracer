#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Vector3f.h>

struct Triangle
{
    Vector3f a;
    float nop1;
    Vector3f b;
    float nop2;
    Vector3f c;
    int index;
    int mat;
    int mat2;
    int mat3;
    int mat4;
};

#endif // TRIANGLE_H
