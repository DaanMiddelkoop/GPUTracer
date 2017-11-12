#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Vector3f.h>

class Triangle
{
    public:
        Triangle(Vector3f a, Vector3f b, Vector3f c);
        Vector3f a;
        Vector3f b;
        Vector3f c;
        int index;

    protected:

    private:
};

#endif // TRIANGLE_H
